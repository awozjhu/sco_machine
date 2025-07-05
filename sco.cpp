/*
Course: EN.605.604.8VL.SU25 Object-Oriented Programming with C++
Semester: Summer 2025
Name: Alex Wozneak
Description: Class SCO (self-checkout) implementation
*/

#include "sco.h"
#include <sstream>
#include <fstream>

// constructor definition 
SCO::SCO(ProductDatabase& pdb, UserDisplay& disp)
    : db(pdb), display(disp) {} // binds reference db to passed-in ProductDatabase object, same concept for display

enum class CommandType { // C++ style scoped enumeration (for type safety and namespace scoping)
    Scan,
    Remove,
    Pay,
    List,
    Help,
    Exit,
    Replenish,
    Unknown
};

// function returns value from scoped enumeration CommandType
CommandType parseCommand(const std::string& cmd) {
    if (cmd == "scan")      return CommandType::Scan;
    if (cmd == "remove")    return CommandType::Remove;
    if (cmd == "pay")       return CommandType::Pay;
    if (cmd == "list")      return CommandType::List;
    if (cmd == "help")      return CommandType::Help;
    if (cmd == "exit")      return CommandType::Exit;
    if (cmd == "replenish") return CommandType::Replenish;
    // unknown command string
    return CommandType::Unknown;
}

// processCommand member function definition: handles each valid input string
void SCO::processCommand(const std::string& input) {
    std::istringstream iss(input);                         // wraps input string into string stream object (iss) for easy parsing
    std::string cmd;
    iss >> cmd;                                            // extracts command portion of the input string 

    CommandType type = parseCommand(cmd);

    if (!inService && type != CommandType::Replenish) {    // don't allow other commands until SCO change repo is replenished 
        display.showMessage("SCO is out of service. Please see an attendant.");
        return;
    }

    switch (type) {
        case CommandType::Scan: {                          // scan item command
            std::string id;
            int qty;
            iss >> id >> qty;                              // extracts product id and quantity portion of input string                          
            if (!db.contains(id)) {                        // check if product id is valid  
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
                double cash;
                iss >> cash;
                double change = 0.0;
                if (Payment::processCash(total, cash, change)) {
                    if (!changeRepo.canDispense(change)) {
                        display.showMessage("Insufficient change in machine. Can't complete transaction!");
                        return;
                    }
                    // checkout steps
                    bool stillInService = changeRepo.dispense(change);
                    if (!stillInService) {
                        inService = false;
                        display.showMessage("SCO is now out of service: change balance low.");
                        // notifyControlCenter();
                    }
                    display.showMessage("*************************");
                    display.showSubtotal(subtotal);
                    display.showTax(tax);
                    display.showTotal(total);
                    display.showChange(change);
                    std::ofstream receipt("receipt.txt");  // create output file stream named receipt, file path receipt.txt
                    cart.printReceipt(receipt);
                    cart = Cart();                         // Clear cart after successful payment
                    display.showMessage("Thanks for shopping!\n");
                } else {
                    display.showMessage("Insufficient cash provided.");
                }
            } else if (method == "card") {                 // pay with card
                std::string code = Payment::processCard(total);
                // checkout steps
                display.showMessage("*************************");
                display.showSubtotal(subtotal);
                display.showTax(tax);
                display.showTotal(total);
                display.showApprovalCode(code);
                std::ofstream receipt("receipt.txt");
                cart.printReceipt(receipt);
                cart = Cart();                             // Clear cart after successful payment
                display.showMessage("Thanks for shopping!\n");
            } else {
                display.showMessage("Unknown payment method.");
            }
            break;
        }

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

        case CommandType::Replenish:
            changeRepo.replenish();
            inService = true;
            display.showMessage("SCO Change replenished and is back in service.");
            break;

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

