#pragma once

#define RED "\033[1;31m"
#define PURPLE "\e[95m"
#define GREEN "\e[92;5;118m"
#define BLUE "\033[1;34m"
#define YELLOW "\e[93;5;226m"
#define GRAY "\e[33;2;37m"
#define RESET "\e[0m"
#define CURSIVE "\e[33;3m"

enum Color {
    R,    // red
    P,    // purple
    G,    // green
    B,    // blue
    Y,    // yellow
    CGR,  // cursive gray
    CB,   // cursive blue
    CG,   // cursive green
    CR    // cursive red
};