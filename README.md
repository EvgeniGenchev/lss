# Simple Ls

A simple replacement for `gnu-ls` written in C that allows for some customization.

![image](https://user-images.githubusercontent.com/59848681/209978967-3289ef5e-5a87-4abf-8d4d-6803bc9f9bf3.png)

## Requirements

It should work on any UNIX x86 system :)

## Features

+ list files data
  + colored file name based on type
  + date of last edit
  + size in human readable format
+ no config file - direct customization via code
+ install script

## How to customize

Similar to `suckless` software the script is contained in one file where you can customize your color palette and extend functionality.

In order to, customize the your `lss` you need to locate the `main.c` file in the repository directory and open it with an editor of choice. There are 3 common customizations.

1. Change the color palette;

   This can be done by editing the `COLOR_XXX` definitions:

   ![image](https://user-images.githubusercontent.com/59848681/209980425-35f03994-1624-4904-8b6c-33178b26d953.png)

   **NOTE**: DO NOT CHANGE THE `COLOR_RESET` VALUE

2. Add more types of a category to be colored:

   This can be done by including more `char *` in the `XXX_ARRAY` definitions and then increasing/decreasing the defined length of the array `XXX_ARR_LEN`. (I am aware setting array length by hand is dumb, however ***~Felt lazy, might patch later ~***):

   ![image](https://user-images.githubusercontent.com/59848681/209980993-09537ab5-707a-41fd-af84-4ce45da5f65a.png)

3. Add more categories

​		In order to add a new category you need to do 3 operations

- Define a new `COLOR_XXX` and `XXX_ARR_LEN`
- Initialize a new global array:

```c
#define COLOR_FONTS #6633FF

#define FONTS_ARR_LEN 4

char * FONTS_ARRAY[] = {".ttf", '.otf', '.ps', '.woff'}
```

- Finally, add the custom array in the `default` case of the   `switch` statement on line `224`:

  ```c
  		switch((S_IFMT & st.st_mode)){ // line 224
  			case S_IFDIR:
  				print_colored(entries[i]->d_name, COLOR_DIR);
  				break;
  			//some switch cases
  
  			default: // line 250
  				if (st.st_mode & S_IXUSR){
  					print_colored(entries[i]->d_name, COLOR_SH);
  				}
  				else{
  					// some if-else statements
  					else if (typeChecker(entries[i]->d_name, FONTS_ARRAY, FONTS_ARR_LEN)){
  						print_colored(entries[i]->d_name, COLOR_FONTS);
  					}
                      // some other if statemetns
  				}
  				break;
  
  ```

   

## Install

In order to install `lss` run the `install.sh` script located in the repository:

```sh
$ sudo ./install.sh
```

Alternatively, you can compile the installation manually:

```bash
$ gcc main.c -o lss
$ sudo cp lss /usr/bin/.
```

