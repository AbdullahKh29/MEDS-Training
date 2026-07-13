# RISC-V Privileged Architecture Summary

## Introduction

The RISC-V Privileged Architecture defines how software interacts with hardware at different privilege levels. It specifies the execution modes, Control and Status Registers (CSRs), interrupt and exception handling, and mechanisms required by operating systems and firmware. Unlike the unprivileged ISA, which defines instructions available to all programs, the privileged architecture manages protected system resources.

---

## Privilege Levels

RISC-V defines three privilege levels:

### Machine Mode (M-mode)

Machine mode is the highest privilege level and is mandatory in every RISC-V implementation. It has unrestricted access to all hardware resources and is responsible for system initialization, interrupt handling, and low-level hardware control. Firmware and bootloaders typically execute in M-mode.

### Supervisor Mode (S-mode)

Supervisor mode is optional and is mainly used by operating system kernels. It manages virtual memory, system calls, processes, scheduling, and protected resource management. S-mode has fewer privileges than M-mode but more than user applications.

### User Mode (U-mode)

User mode provides the lowest privilege level. Applications execute in U-mode and cannot directly access privileged hardware resources or modify privileged CSRs. Any privileged operation attempted in U-mode generates an exception.

---

## Control and Status Registers (CSRs)

CSRs are special-purpose registers used to configure and monitor processor operation.

Some important machine-mode CSRs include:

### `mstatus`

Stores the processor's current status, including interrupt enable bits, previous privilege mode, and other control information.

### `misa`

Describes the processor's supported ISA features, including XLEN (32-bit or 64-bit) and implemented instruction set extensions such as M, A, C, and F.

### `mtvec`

Stores the address of the trap handler. When an interrupt or exception occurs, execution jumps to the address contained in `mtvec`.

### `mepc`

Stores the Program Counter (PC) of the instruction that caused the trap. After servicing the trap, execution can resume from this address using the `mret` instruction.

### `mcause`

Records the reason for the trap. It identifies whether the trap was caused by an interrupt or an exception and specifies its cause.

### `mtval`

Contains additional information related to certain exceptions, such as an invalid instruction or a faulting memory address.

---

## Trap Handling Flow

A trap is any event that transfers control to a privileged trap handler. Traps include both synchronous exceptions and asynchronous interrupts.

The general trap handling process is:

1. A trap (interrupt or exception) occurs.
2. The current PC is saved into `mepc`.
3. The trap cause is recorded in `mcause`.
4. Additional fault information, if available, is stored in `mtval`.
5. Processor status information is updated in `mstatus`.
6. The PC is loaded from `mtvec`, transferring execution to the trap handler.
7. The trap handler identifies the cause, performs the required service, and restores the processor state.
8. The `mret` instruction returns execution to the address stored in `mepc`.

This mechanism allows the processor to safely handle interrupts, exceptions, and system events while preserving program execution.

---

## Conclusion

The RISC-V Privileged Architecture provides a flexible and modular framework for secure system operation. Machine mode provides complete hardware control, Supervisor mode supports operating systems, and User mode safely executes applications. CSRs maintain processor state and configuration, while the trap handling mechanism ensures reliable processing of interrupts and exceptions.