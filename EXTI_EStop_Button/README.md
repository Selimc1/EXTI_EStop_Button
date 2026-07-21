# EXTI Emergency Stop (E-Stop) Button

![STM32](https://img.shields.io/badge/Microcontroller-STM32F407VG-blue)
![Architecture](https://img.shields.io/badge/Architecture-Bare%20Metal-orange)

## 📌 Project Overview
This project simulates an industrial Emergency Stop (E-Stop) safety mechanism using **EXTI (External Interrupts)** on an STM32. Instead of inefficiently polling a button state in an infinite loop, this bare-metal application utilizes hardware-level interrupts. When the button is pressed, the main execution halts, and an immediate safety routine is triggered via the Nested Vectored Interrupt Controller (NVIC).

## 🚀 Key Features
- **Hardware Interrupts (EXTI):** Configured a GPIO pin to trigger a falling-edge/rising-edge hardware interrupt, completely eliminating CPU polling overhead.
- **NVIC Management:** Direct configuration of the Nested Vectored Interrupt Controller registers to enable the EXTI line and set its priority.
- **Instant Response Time:** Guarantees microsecond-level response to the emergency button press.
- **State Indication:** Uses the on-board LEDs (Green and Red) to visually indicate the system state (e.g., Normal Operation vs. Emergency Mode).

## 🛠️ Hardware Requirements
- **STM32F407G-DISC1** Development Board.
- **Push Button** (Typically the on-board USER button connected to PA0).
- **On-board LEDs** (PD12, PD13, PD14, PD15).

## ⚙️ How It Works
1. **Configuration:** The `SYSCFG` module maps the target GPIO pin (e.g., PA0) to the corresponding EXTI line. The EXTI mask (`IMR`) and trigger (`FTSR`/`RTSR`) registers are configured.
2. **Main Loop:** The processor can perform other tasks or enter a low-power sleep mode. The green LED remains on, indicating a safe state.
3. **Interrupt Service Routine (ISR):** When the button is pressed, the hardware automatically jumps to the `EXTI0_IRQHandler`. Inside the handler, the pending bit is cleared, the green LED is turned off, and the red LED is turned on to signal the emergency halt.

## 💻 Getting Started
Open the project in **STM32CubeIDE**. Since this is a bare-metal project, no HAL configurations are required. Simply build the project, load it onto the STM32 via ST-LINK, and press the User Button to observe the hardware interrupt in action.
