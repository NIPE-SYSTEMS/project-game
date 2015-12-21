# Conventions

## Coding style

Use *Allman*-style with the following changes:

* Do not write whitespaces after `while`, `if` or other control statements.
* Do use tabulators as indentation.
* Do not use single line statements (`while`, `if`).

Example of the modified *Allman*-coding-style:

    int f(int x, int y, int z)
    {
        if(x < foo(y, z))
        {
            qux = bar[4] + 5;
            return qux;
        }
        else
        {
            while(z)
            {
                qux += foo(z, z);
                z--;
            }
            return ++x + bar();
        }
    }

## Nomenclature/Naming

### Functions

Names of a function in the code should have the following nomenclature:

    <module name>_<part of the module>_<action to perform>();

#### module name

Each module consists of one source file. It has the name of the module. If the module has multiple words the words are separated by `-`-characters, in the function name `_`-characters. An example file name would be: `image-loader.c`. An example function name can be found in the *action to perform*-paragraph.

#### part of the module

A module can be splitted into multiple parts. The parts are summarized in a single module file. If a part has multiple words the words are separated by `_`-characters. An example function name can be found in the next paragraph. The part can be omitted if the module only has one part.

#### action to perform

The action describes what a function will do with the part in the module. If the action has multiple words the words are separated by `_`-characters. An example function name would be: `image_loader_file_informations_print();`.

### Variables

Do not use global variables. Variables needed in a module can be declared as `static`-variables at the head of the source file. Variables should always be initialized with a default value (at compile time). Variables have the following nomenclature:

    static int <module name>_<part of the module>_<property name> = 0;

*module name* and *part of the module* are described ahead.

#### property name

The property which the variable holds the value for. If the property name has multiple words the words are separated by `_`-characters. An example function name would be: `static int image_loader_file_informations_count = 0;`.

## Other

1. Do not upload binaries.
2. Do describe the commits usefully.
3. Do not commit multiple features in one commit. Split them into several commits.
4. Do not use special characters (whitespaces included):
    * Do not use special characters in the program (code).
    * Do not use special characters in directory names.
    * Do not use special characters in file names.
    * Do not use special characters in commit descriptions.
5. Do use English as general language.
