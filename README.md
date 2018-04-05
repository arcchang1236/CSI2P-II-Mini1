# CSI2P-II-Mini1



## Introduction

* Let’s consider a CPU, which has eight **32 bits registers r0-r7 and a 256 byte memory.**
* In this project, you need to implement a calculator.**The input is a list of expressions consisting of integers, operators(+,-,*,/,=), and three variables x,y, z; and the output is a list of assembly codes.**
* The instructions of the CPU are listed in the **in the table below** The time to execute each instruction (in clock cycles) is also listed.



## Instruction Set Architecture(I)

****

|Opcode|Operand1|Operand2|Meaning|Cycles|
|---|---|---|---|---|
|MOV|Register1|Register2|Move data from ***register2 to register1***|10|
|MOV|Register1|Constant|Set the value of ***register1 constant***|10|
|MOV|Register1|[Addr2]|Move the data (4 bytes) in memory addressed Addr2 to register1. Note that Addr2 must be a ***multiple of 4.***|200|
|MOV|[Addr1]|Register2|Move the data (4 bytes) from register2 to the memory addressed Addr1. Note that Addr1 must be a ***multiple of 4.***|200|



****

## Instruction Set Architecture(II)

****

|Opcode|Operand1|Operand2|Meaning|Cycles|
|---|---|---|---|---|
|ADD|Register1|Register2|**Add the values in register1 to register2** and store the result in **register1**|10|
|SUB|Register1|Register2|**Subtract the value in register2 from the value in register1** and store the result in **register1.**|10|
|MUL|Register1|Register2|**Multiply the values in register1 to register2** and store the result in **register1**|30|
|DIV|Register1|Register2|**Divide the value in register1 by the value in register2** and store the result in **register1**.Note it is the integer division.|50|
|EXIT|Constant||**Stop the program** with a constant signal,whose value is specified as follows. ***0:exit normally***                       ***1:the expression cannot be evaluated***|20|


****

## Variables

* The initial value of variables, **x, y, and z**, are **stored in memory [0], [4], and [8] respectively**. You need to read those initial values first.

* After the evaluation of the assembly code, the answer of the **variables x, y, z needs to be  stored in the registers r0, r1, and r2 respectively**. 


## Example

* **Input: x = z + 5**
* **Output** :

    **MOV r0, [0]**

    **MOV r1, [4]**

    **MOV r2, [8]**

    **MOV r3, 5**

    **ADD r3, r2**

    **MOV r0, r3**

    **EXIT 0**

## Total clock cycles

* Each instruction has an expected runtime, which is specified by clock cycles, as shown in the table.
* The runtime of a program is **the summation of the clock cycles of all instructions.**
* Example: the following code has 90 clock cycles
    MOV r0, 3        **10 cc**

    MOV r1, 5        **10 cc**

    MUL r0, r1       **30 cc**

    MOV r1, 0        **10 cc**

    MOV r2, 0        **10 cc**

    EXIT 0           **20 cc**

## Error handler

* If the expression is **illegal**, such as
    * x = 5 +
    * y = 7 / 0
    * z = --2
    * ... and so on

* **You should consider all kinds of possibilities.**

* Your final output should be **EXIT 1**

## Contest

* The project has 2 parts:
    1. The **5 basic testcases** will be provided by TAs
    2. The contest: There will be **20 testcases** at demo time, each represents 5 points. Besides, the code with the **less total clock cycles** is better. Five top winners will **get extra credits**.

## Submission / Demo

- **Submission Deadline: 4/20(五) 中午 12:00**
	- 上傳至iLMS
	- 命名為 ***學號.c***

- **Demo 時間: 4/20(五) 下午 1:20 ~ 3:30**
    - 請準時出席課堂，我們會唱名請大家來臺前，會使用iLMS上傳的code來Demo
    - Demo完成後會公布過的測資數與cycle數
    - 不要使用別人的code，若抓到抄襲直接零分並依校規處置

