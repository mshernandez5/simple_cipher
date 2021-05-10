#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PASSWORD_BUFFER_SIZE 512
#define CIPHER_BUFFER_SIZE 512

// Checks If The Given Password Meets Certain Requirements
bool isValidPassword(char *password, int bufferSize)
{
    // At Least 20 Characters
    int passLen = strnlen(password, bufferSize);
    if (passLen < 20)
    {
        return false;
    }
    // Character Requirements
    bool uppercase = false;
    bool lowercase = false;
    bool special = false;
    for (char *c = password; *c; c++)
    {
        if (uppercase && lowercase && special)
        {
            break;
        }
        if (*c >= 'A' && *c <= 'Z')
        {
            uppercase = true;
        }
        else if (*c >= 'a' && *c <= 'z')
        {
            lowercase = true;
        }
        else if (!(*c >= '0' && *c <= '9'))
        {
            special = true;
        }
    }
    return uppercase && lowercase && special;
}

// Write Cipher Text: Input File -> XOR Cipher -> Output File
void writeChipherText(FILE *input, FILE *output, char *password)
{
    char buffer[CIPHER_BUFFER_SIZE];
    char *cipherByte = password;
    int bytesRead;
    while ((bytesRead = fread(buffer, sizeof(char), CIPHER_BUFFER_SIZE, input)) != 0)
    {
        char *inputByte = buffer;
        for (int i = 0; i < bytesRead; i++)
        {
            *inputByte++ ^= *cipherByte++;
            if (!(*cipherByte))
            {
                cipherByte = password;
            }
        }
        fwrite(buffer, sizeof(char), bytesRead, output);
    }
}

// Main: Parse Arguments, Open/Close Files
int main(int argc, char **argv)
{
    // Check For Args
    if (argc != 3)
    {
        puts("Incorrect number of arguments!");
        puts("Usage: cipher [input] [output]");
        return 1;
    }

    // Take Password
    char password[PASSWORD_BUFFER_SIZE];
    printf("Enter Password: ");
    fgets(password, PASSWORD_BUFFER_SIZE, stdin);
    password[strcspn(password, "\r\n")] = 0;
    while (!isValidPassword(password, PASSWORD_BUFFER_SIZE))
    {
        puts("Your password does not meet the requirements!");
        puts(" * At Least 20 Characters");
        puts(" * At Least One Uppercase Character");
        puts(" * At Least One Lowercase Character");
        printf("New Password: ");
        fgets(password, PASSWORD_BUFFER_SIZE, stdin);
        password[strcspn(password, "\r\n")] = 0;
    }

    // Open Files
    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL)
    {
        printf("Error Opening Input File: %s\n", argv[1]);
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "wb");
    if (outputFile == NULL)
    {
        printf("Error Opening Ouput File: %s\n", argv[2]);
        return 1;
    }

    // XOR Cipher
    writeChipherText(inputFile, outputFile, password);
    
    // Close Files
    fclose(inputFile);
    fclose(outputFile);

    // Exit
    return 0;
}