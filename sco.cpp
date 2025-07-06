/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: Class SCO (self-checkout) implementation
*/

#include "sco.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <iomanip>

// hardcoded password for sco replenish command
const std::string REPLENISH_PASSWORD = "foodisgood123";

// constructor definition 
SCO::SCO(ProductDatabase& pdb, UserDisplay& disp)
    : db(pdb), display(disp) { // binds reference db to passed-in ProductDatabase object, same concept for display
    inService = !changeRepo.isLow();
    } 

enum class CommandType { // C++ style scoped enumeration (for type safety and namespace scoping)
    Scan,
    Remove,
    Pay,
    List,
    Cancel,
    Help,
    Exit,
    Replenish,
    ShowChange,
    Unknown
};

// function returns value from scoped enumeration CommandType
CommandType parseCommand(const std::string& cmd) {
    if (cmd == "scan")        return CommandType::Scan;
    if (cmd == "remove")      return CommandType::Remove;
    if (cmd == "pay")         return CommandType::Pay;
    if (cmd == "list")        return CommandType::List;
    if (cmd == "cancel")      return CommandType::Cancel;
    if (cmd == "help")        return CommandType::Help;
    if (cmd == "exit")        return CommandType::Exit;
    if (cmd == "replenish")   return CommandType::Replenish;
    if (cmd == "show change") return CommandType::ShowChange;
    // unknown command string
    return CommandType::Unknown;
}

// helper functions to clean things up
int dollarsToCents(double amount) {
    return static_cast<int>(std::round(amount * 100));
}

double centsToDollars(int amount) {
    return static_cast<double>(amount) / 100.0;
}

std::string formatAmount(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

void SCO::goodbyeReset() {
    display.showMessage("Thanks for shopping!\n");
    std::this_thread::sleep_for(std::chrono::seconds(4));
    display.showWelcome();
}

void SCO::checkoutMessages(double subtotal, double tax, double total) {
    display.showMessage("*************************");
    display.showSubtotal(subtotal);
    display.showTax(tax);
    display.showTotal(total);
}

// generate receipt member function: writes receipt to text file
void SCO::generateReceipt(double total, double paid, double change) {
    std::ofstream receipt("receipt.txt");
    cart.printReceipt(receipt, total, paid, change);
}  

// processCommand member function definition: handles each valid input string
void SCO::processCommand(const std::string& input) {
    std::istringstream iss(input);                         // wraps input string into string stream object (iss) for easy parsing
    std::string word1, word2;
    iss >> word1;                                          // extracts 1st word of input string command

    if (word1 == "show") {
        iss >> word2;
        word1 += " " + word2;                              // only join strings if first word is "show"
    }

    CommandType type = parseCommand(word1);                // parse the input command

    if (!inService && type != CommandType::Replenish && type != CommandType::ShowChange) { // don't allow other commands until SCO change repo is replenished
        display.showMessage("ERROR: SCO is out of service. Please see an attendant.");
        return;
    }

    switch (type) {
        case CommandType::Scan: {                          // scan item command
            std::string id;
            int qty;
            iss >> id >> qty;                              // extracts product id and quantity portion of input string                          
            if (!db.contains(id)) {                        // check  if product id is valid  
                display.showMessage("Unknown product ID.");
                return;
            }
            Product p = db.lookup(id);
            cart.addItem(p, qty);
            display.showScanned(p, qty);   
            display.showSubtotal(cart.getSubtotal());
            break;
        }

        case CommandType::Remove: {                        // remove item command
            int itemNum;
            iss >> itemNum;
            cart.removeItem(itemNum);
            display.showMessage("Item removed from cart.");
            display.showSubtotal(cart.getSubtotal());
            break;
        }
                                           
        case CommandType::Pay: {                           // pay for cart command
            std::string method;
            iss >> method;
            double subtotal = cart.getSubtotal();
            double tax = subtotal * taxRate;
            double total = subtotal + tax;

            if (cart.isEmpty()) {
                display.showMessage("Cart is empty. Scan items first.");
                return;
            }

            if (method == "cash") {                       // pay with cash
                // double cash;
                std::map<int, int> inserted;              // stores cash denominations & qty entered by user
                std::string line;
                double insertedTotal = 0.0;               // total cash inserted 

                display.showMessage("Enter denomination and quantity (e.g., 0.25 4 for four quarters), or type 'done' when finished:");

                while (true) {                            // loop until user enters "done"
                    display.cashprompt();
                    std::getline(std::cin, line);         // get user input
                    std::istringstream input(line);       // put user input into a string stream for parsing

                    std::string word;
                    input >> word;                        // extract first word from input string

                    if (word == "done") {
                        if (insertedTotal >= total) {
                            break;                        // exit loop, sufficient funds to complete payment 
                        } else {
                            double remaining = total - insertedTotal;
                            display.showMessage("Still owed: $" + formatAmount(remaining) + ". Please insert more cash.");
                            continue;                     // stay in loop until sufficient funds are inserted 
                        }
                    }
                      
                    try {
                        double denomDollars = std::stod(word);                  // convert string to decimal (e.g., "0.25" --> 0.25)
                        int denom = dollarsToCents(denomDollars);               // Convert dollars to cents (integers) (e.g., 0.25 --> 25)
                        int qty;
                        input >> qty;                                           // extract quantity from input string

                        // check if cash input is valid
                        int validDenoms[] = {1, 5, 10, 25, 100, 500, 1000, 2000, 5000, 10000}; // TODO try to implement with std::set
                        int numDenoms = sizeof(validDenoms) / sizeof(validDenoms[0]);

                        bool isValid = false;
                        for (int i = 0; i < numDenoms; ++i) {
                            if (denom == validDenoms[i]) {
                                isValid = true;
                                break;
                            }
                        }

                        if (!isValid || qty <= 0) {                             // check if invalid denom or quantity input
                            throw std::invalid_argument("Invalid denomination or quantity");
                        }

                        inserted[denom] += qty;                                 // accumulate inserted qty
                        insertedTotal += (denom / 100.0) * qty;                 // update total inserted 

                        std::ostringstream oss;
                        oss << "Added $" << std::fixed << std::setprecision(2)
                            << (denom / 100.0) * qty
                            << " (total so far: $" << insertedTotal << ")";
                        display.showMessage(oss.str());                         // show how much was added to the new total 

                        double remaining = total - insertedTotal;   
                        if (remaining > 0) {  
                            display.showMessage("Still owed: $" + formatAmount(remaining)); // show remaining balanced owed
                        }

                    } catch (...) {
                        display.showMessage("Invalid input. Try again.");       // for any conversion or logic errors  
                    }
                }

                double change = 0.0;
                if (Payment::processCash(total, insertedTotal, change)) {       // check if cash covers total, generate change
                    if (!changeRepo.canDispense(change)) {                      // check to see if change repo has sufficient change
                        display.showMessage("Insufficient change in machine. Can't complete transaction!");
                        return;
                    }

                    // Attempt to dispense change (before updating cash)
                    auto dispensed = changeRepo.dispenseChange(dollarsToCents(change)); // dispense change
                    if (dispensed.empty()) {
                        display.showMessage("Unable to dispense exact change. Transaction cancelled.");
                        return;
                    }

                    // Save only the cash that is actually retained (total amount due)
                    changeRepo.acceptInsertedCash(inserted, dollarsToCents(total));

                    // Check for low balance after dispensing change
                    if (changeRepo.isLow()) {
                        inService = false;
                        display.showMessage("\nERROR: SCO is now out of service: change balance below $50 !!!!");
                        std::this_thread::sleep_for(std::chrono::seconds(3)); // delay for 3s
                    }

                    //// checkout messages
                    checkoutMessages(subtotal, tax, total);
                    display.showChange(change);
                    display.showChangeBreakdown(dispensed);
                    // print receipt 
                    generateReceipt(total, insertedTotal, change); // generate receipt 
                    cart = Cart();         // empty cart after successful payment
                    // goodbye message and reset sco
                    goodbyeReset();
                } else {
                    display.showMessage("Insufficient cash provided.");
                }

            } else if (method == "card") {                 // pay with card
                display.showCardProcessingAnimation();
                std::string code = Payment::processCard(total);
                //// checkout messages
                checkoutMessages(subtotal, tax, total);  
                // card approval code
                std::this_thread::sleep_for(std::chrono::seconds(3));
                display.showApprovalCode(code);
                std::this_thread::sleep_for(std::chrono::seconds(3));
                // print receipt 
                generateReceipt(total, total, 0.0); // generate receipt 
                cart = Cart();         // empty cart after successful payment
                // goodbye message and reset sco
                goodbyeReset(); 
            } else {
                display.showMessage("Unknown payment method.");
            }
            break;
        }
 
        case CommandType::Cancel:                        // cancel transaction
            if (cart.isEmpty()) {
                display.showMessage("Cart is already empty.");
            } else {
                cart = Cart(); // reset to new cart object
                display.showMessage("Transaction cancelled. Cart emptied.");
            }
            display.showSubtotal(0.0);
            break;

        case CommandType::List:                          // list cart items command
            if (cart.isEmpty()) {
                display.showMessage("Cart is empty.");
            } else { 
                cart.printItems(); 
            }
            display.showSubtotal(cart.getSubtotal());
            break;

        case CommandType::Help:                           // help command (see command list)
            display.showHelp();  
            break;

        case CommandType::Exit:                           // exit command (terminate program)
            display.showMessage("Stopping Application...");
            display.showMessage("Goodbye!");
            std::exit(0);

        case CommandType::Replenish: {
            // password protect the replenish command
            std::string enteredPassword;
            display.showMessage("Enter admin password to replenish:");
            display.prompt();
            std::getline(std::cin, enteredPassword);

            if (enteredPassword != REPLENISH_PASSWORD) {              // check password
                display.showMessage("Incorrect password. Access denied.\n");
                break;
            }

            if (changeRepo.replenish(display)) {                      // replenish cash denominations in change repo
                inService = true;
                display.showMessage("SCO replenished and back in service.\n");
                std::this_thread::sleep_for(std::chrono::seconds(4)); // delay for 4s
                display.showWelcome();
            } else {
                display.showMessage("Replenishment not allowed yet. Please wait 24 hours.\n");
                std::this_thread::sleep_for(std::chrono::seconds(4)); // delay for 4s
                display.showWelcome();  
            }
            break;
        }
        case CommandType::ShowChange: {
            int cents = changeRepo.getTotalBalance();
            double dollars = centsToDollars(cents);  
            display.showMessage("Change repository total: $" + formatAmount(dollars));
            break;
        }

        case CommandType::Unknown:
        default:
            display.showMessage("Unknown command. Type 'help' for list of commands.");
            break; 

    }    
}

// void SCO::notifyControlCenter() {
//     std::ofstream log("sco_alert.log", std::ios::app);
//     log << "SCO out of service at " << __TIME__ << ": low change balance.\n";
// }

