# RZBuddy M33 Application

![RZBuddy Logo](path_to_logo_image)

## Overview

**RZBuddy M33** is the ARM Cortex-M33 application logic for the **RZBuddy Pet Treat Dispenser** device. This application controls the hardware logic, interfacing with the ARM Cortex-A55 cores through shared memory and peripherals to coordinate the dispensing actions.

Developed for the **RZBoard V2L Evaluation Kit**, the application leverages the Renesas Flexible Software Package (FSP) version 1.1.0 and utilizes the GCC for Renesas RZ toolchain.

## Table of Contents

- [Project Specifications](#project-specifications)
- [Software Components](#software-components)
- [Stacks and Dependencies](#stacks-and-dependencies)
- [Getting Started](#getting-started)
- [License](#license)
- [Contact](#contact)

## Project Specifications

| Specification            | Details                             |
| ------------------------ | ----------------------------------- |
| **Board**                | RZ/V2L Evaluation Kit (SMARC)       |
| **Device**               | R9A07G054L23GBG_CM33                |
| **Toolchain**            | GCC for Renesas RZ                  |
| **Toolchain Version**    | 9.2.1.20191025                      |
| **FSP Version**          | 1.1.0                               |
| **Project Type**         | Flat                                |

## Software Components

The project incorporates the following software components:

- **Core Components:**
  - Arm CMSIS Version 5 - Core (M) v5.7.0
  - FreeRTOS v1.1.0
  - FreeRTOS Port v1.1.0

- **Board Support Packages:**
  - Evaluation Kit RZ/V2L Support Files v1.1.0
  - Board support package for R9A07G054L23GBG_CM33 v1.1.0
  - Board support package for RZV2L v1.1.0
  - Board support package for RZ/V2L (RZ/V2L) - FSP Data v1.1.0
  - Board Support Package Common Files v1.1.0

- **Peripheral Drivers:**
  - General Timer v1.1.0
  - General PWM Timer v1.1.0
  - I/O Port v1.1.0

- **Utilities:**
  - Pin Configuration Template File v1.1.0

## Stacks and Dependencies

- **Component Dependency:**
  - **General Purpose Timer (r_gpt) v1.1.0**

- **Stack Configuration:**
  - **GPT Driver:** Named `g_timer_servo`, outputs on **Channel 7 A**.
    - **Period:** 20ms
    - **Duty Cycle:** 8.5%

## Getting Started

### Prerequisites

- **Hardware:**
  - RZ/V2L Evaluation Kit (SMARC)
  
- **Software:**
  - [e2 studio](https://www.renesas.com/us/en/software-tool/e-studio) IDE
  - GCC for Renesas RZ Toolchain Version 9.2.1.20191025
  - Renesas FSP Version 1.1.0

### Setup Instructions

#### Software

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/rzbuddy_m33.git
   ```

2. **Import the Project into e2 studio:**

Open e2 studio.
Navigate to File > Import > Existing Projects into Workspace.
Browse to the cloned repository location.
Select the rzbuddy_m33 project and import.

3. **Build the Project:**

Right-click on the project in the Project Explorer.
Select Build Project.

4. **Flash the Application:**

Connect the RZBoard to your SEGGER J-Link debugger, connected to your machine running e2 studio.

5. **Run and Test:**

Modify your debug configuration according to Renesas documentation. Don't forget to configure your vector & secure vector values.

#### Hardware

A top-level view of the RZBuddy Pet Treat Dispenser hardware will be provided in the future.

If using the default RZBuddy config, make sure your servo data line is connected to gpio port 12 pin 0, and ground is connected properly for the servo.


## License

This project has several licenses. You will need to review and comply with the licenses for each component.

Newly developed / non-generated code is found under `src/` and is licensed under **MIT License**.

The Renesas Flexible Software Package (FSP) License files can be found at various paths (Look for LICENSE.txt) or LICENSE.md files in the project.

## Contact

For any inquiries or support, please reach out through github first.

