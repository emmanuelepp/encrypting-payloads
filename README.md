# Encrypting Payloads

This repository provides multiple methods and examples for encrypting your implants.

## XOR Encryption

### How to Test the `xor_encrypt` Function

### Compile the Program
```bash
g++ -o xorencrypt xor_encrypt.cpp
```
### Create a Test File or Use Another One
```bash
echo "My hyper duper cool payload!" > test.bin
```
### Execute the Program
```bash
./xorencrypt test.bin
```
### View the Output
```bash
{ 0x1f, 0x2a, 0x3b, 0x4c, 0x5d, 0x6e, 0x7f, 0x80, ... }
```

