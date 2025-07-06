# Self-Checkout Machine (SCO) Program

A terminal-based C++ application simulating the core functionality of a self-checkout machine.

## Features

- Scan and remove products by ID
- subtotal and tax calculation
- Pay with **cash** (denomination-by-denomination) or **card**
- Dispense exact change (if available)
- Automatically disables machine if change repository falls below $50
- "Replenish" command to restock cash (admin only, password protected)
- Generate receipts (printed to `receipt.txt`)
- Show current change repository balance
- Cancel transaction at any time

## Build & Run

### Prerequisites
- C++17 compiler
- `g++` should have access to standard headers like `<chrono>`, `<thread>`, `<map>`, etc.

### Compile
```bash
g++ -std=c++17 -g main.cpp sco_application.cpp sco.cpp cart.cpp product.cpp product_database.cpp user_display.cpp payment.cpp change_repository.cpp -o sco
```

### Run
```bash
./sco
```

## Example Commands

```
scan Bread01 2
remove 1
list
pay cash
cancel
replenish
show change
help
```

## Admin Access
- `replenish` is password-protected (hardcoded password prompt)
    *** the password is "foodisgood123"
- Change repository auto-restocks after 24 hours (if allowed)

## Output
- `receipt.txt` is generated for every completed purchase
- Displays total, amount paid, and change
