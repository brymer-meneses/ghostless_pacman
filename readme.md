---
author: Brymer Meneses and Aaron Paul Gapud
title: Introduction to Ghostless Pacman
geometry: margin=2cm
---

![](https://i.imgur.com/QEg8UpR.png)

# Introduction
## I. Pre-play Algorithm

1. Application launches

2. User chooses from the menu options:
    a. If the user presses 1, the *Start* option will be highlighted
        - If the user presses Enter, proceed to Step 3
    b. If the user presses 2, the *Tutorial* option will be highlighted
        - If the user presses Enter, proceed to Step 4
    c. If the user presses 3, the *Exit* option will be highlighted
        - If the user presses Enter, proceed to Step 5
    d. If the user presses A, the *About the Game* option will be highlighted
        - If the user presses Enter, proceed to Step 3
    e. Else, display a wrong input reminder.

3. User chooses the number of food pieces s/he wants for the game
	a. If the user input is from 2 to 9, display chosen number
		- If the user presses Enter, proceed to **In-play Algorithm**
	b. If the user presses M, go back to Step 2
	c. Else, display a wrong input reminder

4. Program shows the first slide of the *Tutorial*
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
    e. If the user presses M, the program goes back to menu (go back to *Step 2 of the **Pre-play Algorithm***)
    f. If the user presses X, the program prompts a quit window (recall *Step 5 of the **Pre-play Algorithm***)
        - If the user presses Y, exit the program
        - If the user presses N, the prompt closes
	g. Else, display a wrong input reminder
2. User plays the game by moving Pacman to eat the food pieces
	a. If Pacman hits a block, display a Game Over prompt
	b. If Pacman gets out of the board, display a Game Over prompt
	c. If Pacman reaches the door without eating all the food pieces, display a Game Over prompt
	d. If Pacman reaches the door after eating all the food pieces, display a Game Victory prompt

3. User chooses between the options to restart, return to menu, or exit the game
	a. If the user presses R, restart the game (go back to *Step 3 of the **Pre-play Algorithm***)
	b. If the user presses M, return to menu (go back to *Step 2 of the **Pre-play Algorithm***)
	c. If the user presses X, the program prompts a quit window (recall *Step 5 of the **Pre-play Algorithm***)
        - If the user presses Y, exit the program
        - If the user presses N, the prompt closes
    d. Else, display a wrong input reminder

# Error handling

As seen in the algorithms, the game was programmed to handle errors of user input by displaying wrong input reminders per various instances.

To recall, below is a list of these occurrences:

- In *Menu*, the program displays a reminder when the user keypress is not '1', '2', '3', or 'A'
- In the *Game*
	- In choosing the food number, the program displays a reminder when the user chooses a number outside the range from 2 to 9
	- In playing the game, the program displays a reminder when the user presses keys other than 'W', 'S', 'A', or 'D' to move Pacman, and when the user presses keys other than 'M' to return to menu or 'X' to exit the game
	- In choosing an option after the game results, displayed through the game prompts, the program displays a reminder when the user presses keys other than 'R' to restart, 'M' to return to menu, or 'X' to exit
- In *Tutorial*
    - In navigating, the program displays a reminder when the user presses keys other than '←' or '→' to navigate through the tutorial slides, when the user presses keys other than 'M' to return to menu
    - In the last slide, the program displays a reminder when the user presses keys other than '1' to start the game, 'M' to return to menu, or 'X' to exit
- In *About the Game*, the program displays a reminder when the user presses keys other than 'M' to return to menu


> Choose Start!
> Enter the number of food you want Pacman to eat.
> Move Pacman using W, A, S, and D, or visit the Tutorial from the menu to learn.
> You have to eat all the pieces before going to the door.
> Don’t miss one! Don’t hit a block or get out of the box!
> But do all that you can to win in the Ghostless Pacman.
> And remember, you can always play again.


## Compilation

# Description

# Algorithm

# Error handling

# Manual
