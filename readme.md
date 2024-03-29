![](https://i.imgur.com/QEg8UpR.png)

# Introduction

Ghostless Pacman is a 2021 adaptation of the video game series Pac-man™, only
without the, as stated, ghosts. It was mainly recreated using the C language by
Brymer Meneses and Paul Gapud, freshman Computer Science students from the
University of the Philippines - Baguio.

The program mainly utilized the concept of arrays to return a working 10-by-10
game board. To improve user experience, visual and audio elements were added to
the program interface. This feature is the significant purpose of using a
library called _SDL_ or _[Simple DirectMedia Layer](https://www.libsdl.org/)_.

![Image of Pacman, in the Ghostless Pacman game](https://i.imgur.com/wP7ynGR.png)

Here is Pacman, who looks more like Pacman than a plain 'O'. In fact, this
Pacman actually munches. Rendering its animation is a feature of the said SDL
library. The same feature allowed the program to display food pieces as
something more delicious than asterisks or any other punctuation.

![Image of Food, in the Ghostless Pacman game](https://i.imgur.com/jppJPvC.png)

Game obstacles that look tougher than 'X'.

![Image of Block, in the Ghostless Pacman game](https://i.imgur.com/10PmFAi.png)

And instead of a dollar sign for the exit door, here's an exit door.

![Image of Door, in the Ghostless Pacman game](https://i.imgur.com/d2olp1M.png)

# Manual

Upon opening the application, the user can navigate through it intuitively. The program also provides instructions and reminders within itself. Nonetheless, here is a quick guide.

> Choose from the menu:
>
> 1. Press 1 to _Start._ You will be prompted to pick a food number from 2-9. Hit enter, and the game begins! Move Pacman using W, A, S, and D, or visit the Tutorial from the menu to learn. You have to eat all the pieces before going to the door. Don’t miss one! Don’t hit a block or get out of the box! After a game, you can always play again.
>
> 2. Press 2 to _Tutorial._ Navigate through the slides by using arrow left (←) and arrow right (→)
>
> 3. Press 3 to _Exit._ Confirm by pressing Y, or deny by pressing N.
>
> 4. _(Extra!)_ Press A to know what's _About the Game._

# Compiling

## Windows

### Installing CMake and GNU Make

To compile this application, you need to have **CMake** and **GNU Make** on your system. The easiest way to do this is by using the **scoop package manager**. **Scoop** is a package manager for windows that automates the installation of various software using commands. You can learn more about it here

To do this open powershell run the following command.

```bash
iwr -useb get.scoop.sh | iex

```

If you get an error you might need to change the execution policy (i.e. enable Powershell), to do so run the following command and redo the command above.

```bash
Set-ExecutionPolicy RemoteSigned -scope CurrentUser
```

Confirm that you have successfully installed scoop, by running the following command”

```bash
scoop --help
```

Once you see you have successfully installed **scoop**.

```bash
Usage: scoop <command> [<args>]

Some useful commands are:

alias       Manage scoop aliases
bucket      Manage Scoop buckets
cache       Show or clear the download cache
```

Now to install **CMake** and **GNU Make** run the following command:

```bash
scoop install cmake make
```

To check whether the installation is successful for **GNU Make** run the following command:

```bash
make --help
```

You will be able to see the following:

```
Usage: make [options] [target] ...
Options:
  -b, -m                      Ignored for compatibility.
  -B, --always-make           Unconditionally make all targets.
  -C DIRECTORY, --directory=DIRECTORY
                              Change to DIRECTORY before doing anything
```

To check whether the installation is successful for **CMake** run the following command:

```bash
cmake --help
```

You will be able to see the following:

```bash
Usage

  cmake [options] <path-to-source>
  cmake [options] <path-to-existing-build>
  cmake [options] -S <path-to-source> -B <path-to-build>

Specify a source directory to (re-)generate a build system for it in the
current working directory.  Specify an existing build directory to
re-generate its build system.
```

### Compiling the game

Once you have installed **CMake** and **GNU Make**, you may now compile the application. To do so, navigate to the _ghostless-pacman_ folder and run.

```bash
make build
```

This command will automatically download external GUI libraries which were used in making the game. After that it will compile the program and place it to the _bin_ folder.

### Running the game

Now the only thing left is to run the application, you can navigate to the bin folder and open the `ghostless-pacman.exe` file which will run the game. Or you can run the following command to do this automatically:

```bash
make run
```

**NOTE**: It is important to avoid moving the executable since Windows will not be able to find the DLLs which are required for running the game.

## Linux

Use your distribution’s package manager to install the following packages:
-SDL2
-SDL2_Mixer
-SDL2_Image

Navigate to the the `ghostless-pacman` folder and run:

```bash
make build
```

## MacOS

Use brew to install the following dependencies:
```bash
brew install sdl2 sdl2_image sdl2_mixer
```

Navigate to the the `ghostless-pacman` folder and run:

```bash
make build
```


# Algorithm

## I. Pre-play Algorithm

1. Application launches

2. User chooses from the menu options:
   a. If the user presses 1, the _Start_ option will be highlighted - If the user presses Enter, proceed to Step 3
   b. If the user presses 2, the _Tutorial_ option will be highlighted - If the user presses Enter, proceed to Step 4
   c. If the user presses 3, the _Exit_ option will be highlighted - If the user presses Enter, proceed to Step 5
   d. If the user presses A, the _About the Game_ option will be highlighted - If the user presses Enter, proceed to Step 3
   e. Else, display a wrong input reminder.

3. User chooses the number of food pieces s/he wants for the game
   a. If the user input is from 2 to 9, display chosen number - If the user presses Enter, proceed to **In-play Algorithm**
   b. If the user presses M, go back to Step 2
   c. Else, display a wrong input reminder

4. Program shows the first slide of the _Tutorial_
   a. User navigates through the slides by pressing arrow left (←) and arrow right (→)
   b. If the user presses M, go back to Step 2
   c. If the user presses 1, proceed to Step 3
   d. If the user presses X, proceed to Step 5
   d. Else, display a wrong input reminder
5. Program prompts a quit window
   a. If the user presses Y, exit the program
   b. If the user presses N, the prompt closes

## II. In-play Algorithm

1. Display a 10-by-10 board, with Pacman on the top left, with the food pieces based on the accepted user input, randomly distributed blocks, and randomly placed exit door
   a. If the user presses W, pacman moves up
   b. If the user presses S, pacman moves down
   c. If the user presses A, pacman moves left
   d. If the user presses D, pacman moves right
   e. If the user presses M, the program goes back to menu (go back to \*Step 2 of the **Pre-play Algorithm\***)
   f. If the user presses X, the program prompts a quit window (recall \*Step 5 of the **Pre-play Algorithm\***) - If the user presses Y, exit the program - If the user presses N, the prompt closes
   g. Else, display a wrong input reminder
2. User plays the game by moving Pacman to eat the food pieces
   a. If Pacman hits a block, display a Game Over prompt
   b. If Pacman gets out of the board, display a Game Over prompt
   c. If Pacman reaches the door without eating all the food pieces, display a Game Over prompt
   d. If Pacman reaches the door after eating all the food pieces, display a Game Victory prompt

3. User chooses between the options to restart, return to menu, or exit the game
   a. If the user presses R, restart the game (go back to \*Step 3 of the **Pre-play Algorithm\***)
   b. If the user presses M, return to menu (go back to \*Step 2 of the **Pre-play Algorithm\***)
   c. If the user presses X, the program prompts a quit window (recall \*Step 5 of the **Pre-play Algorithm\***) - If the user presses Y, exit the program - If the user presses N, the prompt closes
   d. Else, display a wrong input reminder

# Error handling

As seen in the algorithms, the game was programmed to handle errors of user input by displaying wrong input reminders per various instances.

To recall, below is a list of these occurrences:

- In _Menu_, the program displays a reminder when the user keypress is not '1', '2', '3', or 'A'
- In the _Game_
  - In choosing the food number, the program displays a reminder when the user chooses a number outside the range from 2 to 9
  - In playing the game, the program displays a reminder when the user presses keys other than 'W', 'S', 'A', or 'D' to move Pacman, and when the user presses keys other than 'M' to return to menu or 'X' to exit the game
  - In choosing an option after the game results, displayed through the game prompts, the program displays a reminder when the user presses keys other than 'R' to restart, 'M' to return to menu, or 'X' to exit
- In _Tutorial_
  - In navigating, the program displays a reminder when the user presses keys other than '←' or '→' to navigate through the tutorial slides, when the user presses keys other than 'M' to return to menu
  - In the last slide, the program displays a reminder when the user presses keys other than '1' to start the game, 'M' to return to menu, or 'X' to exit
- In _About the Game_, the program displays a reminder when the user presses keys other than 'M' to return to menu
