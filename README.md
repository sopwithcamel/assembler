* Time to solve: 2hrs
* Add Mult. and Div. support: 10 mins
* Clean up + documentation: 20 mins

Assumptions:
* Operands are letters between A and Z
* Program handles spaces in input and empty lines
* Assume that maximum length is 4096

High-level explanation:

The program models a state machine. The state machine has two states: empty and
loaded (which refer to the state of the register). In each case, it handles
inputs which can be either operands or operators.

Transition table:

    State       Input               Output state          Output action
    -----------------------------------------------------------------------
    
    EMPTY       Operand             EMPTY                 Load operand
    EMPTY       Operator            ERROR                   -
    LOADED      Operand&Operator    LOADED                Apply operator to input             
    LOADED      Operand&Operand     LOADED                Store into temp and load new operand
    LOADED      Operator            LOADED                Apply operator to latest temp. variable


