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
- [Installing](#installing)
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

## Installing

There are two primary methods to install the application:

1. **Flash via Debugger**

   Connect the RZBoard to your SEGGER J-Link debugger, connected to your machine running e2 studio. Build the project and flash the application to the board.

2. **Load to RAM via UBoot**

    In general, you need these 4 files in eMMC or uSD:

    1. <NAME>_secure_vector.bin
    2. <NAME>_secure_code.bin
    3. <NAME>_non_secure_vector.bin
    4. <NAME>_non_secure_code.bin

    The procedure is like: get m33 files on your non-volatile storage, boot RZBoard into UBoot, and load the applications to RAM:

    The following commands are sufficient if you're storing the files in eMMC device 0 partition 1:
    > [!IMPORTANT]
    > Replace `<NAME>` with the name of the application.

    ```bash
    dcache off
    mmc dev 0
    fatload mmc 0:1 0x0001FF80 <NAME>_secure_vector.bin
    fatload mmc 0:1 0x42EFF440 <NAME>_secure_code.bin
    fatload mmc 0:1 0x00010000 <NAME>_non_secure_vector.bin
    fatload mmc 0:1 0x40010000 <NAME>_non_secure_code.bin
    cm33 start_debug 0x1001FF80 0x00010000
    dcache on
    ```
    
    Personally, I set UBoot like the following:
    ```bash
    setenv bootcm33 'dcache off; mmc dev 0; fatload mmc 0:1 0x0001FF80 rzbuddy_m33_secure_vector.bin; fatload mmc 0:1 0x42EFF440 rzbuddy_m33_secure_code.bin; fatload mmc 0:1 0x00010000 rzbuddy_m33_non_secure_vector.bin; fatload mmc 0:1 0x40010000 rzbuddy_m33_non_secure_code.bin; cm33 start_debug 0x1001FF80 0x00010000; dcache on;'
    setenv bootcmd 'run envboot; if test "${run_cm33}" = "1" || test "${run_cm33}" = "yes"; then run bootcm33; fi; mmc dev ${mmcdev}; if mmc rescan; then if run loadimage; then run mmcbootdto; else run netboot; fi; fi; run bootimage'
    saveenv
    ```

    Next, I modify /boot/uEnv.txt to include the following:
    ```bash
    run_cm33=yes
    ```

    Lastly, I house the 4 files in /boot/ like the following:
    ```bash
    root@rzboard:~# ls -l /boot/
    ....
    -rwxr-xr-x 1 root root    28120 Aug 26 21:38 rzbuddy_m33_non_secure_code.bin
    -rwxr-xr-x 1 root root     1984 Aug 26 21:38 rzbuddy_m33_non_secure_vector.bin
    -rwxr-xr-x 1 root root      400 Aug 26 21:38 rzbuddy_m33_secure_code.bin
    -rwxr-xr-x 1 root root       64 Aug 26 21:38 rzbuddy_m33_secure_vector.bin
    -rwxr-xr-x 1 root root      273 Aug 26 23:45 uEnv.txt
    ```
    

Option `1.` temporary, while `2.` is permanent.

## License

This project has several licenses. You will need to review and comply with the licenses for each component.

Newly developed / non-generated code is found under `src/` and is licensed under **MIT License**.

The Renesas Flexible Software Package (FSP) License files can be found at various paths (Look for LICENSE.txt) or LICENSE.md files in the project.

## Contact

For any inquiries or support, please reach out through github first.

