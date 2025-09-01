# 🌾 Estação Agroclimática Portátil # 

Este repositório contém o código-fonte e recursos para a implementação de uma **Estação Agroclimática Portátil**, desenvolvida como uma solução de monitoramento ambiental para a agricultura familiar. O sistema coleta, exibe e armazena dados climáticos essenciais (temperatura, umidade, luminosidade e pressão atmosférica) em tempo real, fornecendo informações críticas para o planejamento agronômico e tomada de decisões no campo.

## 🚀 Funcionalidades

- Leitura de sensores ambientais através da interface I2C (AHT10, BH1750 e BMP280)
- Exibição dos dados dos sensores em display OLED
- Armazenamento dos dados no cartão SD via SPI
- Suporte ao sistema operacional em tempo real (FreeRTOS)
- Estrutura modular facilitando expansão de funcionalidades (Wi-Fi, interface web, etc.)

## 📦 Organização do Projeto

- `drivers/` e `hal/` – Implementação dos drivers e camadas de abstração dos sensores e periféricos.
- `lib/` – Funções de configuração de hardware e manipulação do cartão SD.
- `src/` – Arquivo principal do projeto (`main.c`).
- `include/` – Headers dos sensores, periféricos e configurações.
- `FreeRTOS-Kernel/` – Kernel do sistema operacional em tempo real (necessário clonar).
- `no-OS-FatFS-SD-SPI-RPi-Pico/` – Biblioteca para comunicação com cartão SD (necessário clonar).

## ⚙️ Como Compilar

Antes de compilar, é necessário clonar os repositórios de dependências na raiz do projeto:


`git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git`

`git clone https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico`


O projeto utiliza `CMake` como sistema de build. Para compilar, execute:

mkdir build
cd build
cmake ..
make

## 📂 Estrutura do Projeto  
```  
├── app/
│ └── main.c # Programa principal
├── drivers/
│ ├── ssd1306.c # Driver OLED SSD1306
│ ├── ssd1306.h
│ ├── ssd1306_i2c.c
│ ├── ssd1306_i2c.h
│ └── ssd1306_font.h
├── hal/
│ ├── AHT10.c # Leitura AHT10
│ ├── BH1750.c # Leitura BH1750
│ ├── BMP280.c # Leitura BMP280
│ ├── buttons.c # Botoes A e B
│ ├── i2c_setup.c # Configura porta i2c
│ └── display.c # Funções do display OLED
├── include/
│ ├── AHT10.h
│ ├── BH1750.h
│ ├── BMP280.h
│ ├── buttons.h
│ ├── i2c_setup.h
│ ├── FreeRTOSConfig.h
│ ├── lwipopts.h
│ └── display.h
├── lib/
│ ├── hw_config.h
│ ├── sd_card.c
│ └── sd_card.h
├── no-OS-FatFS-SD-SPI-RPi-Pico/
├── CMakeLists.txt
└── README.md
```  
---

## 📊 Funcionamento  

  - Lê os sensores 
  - Exibe na tela old a tela 1, onde mostra os estatus dos sensores conectados

    ```
    SD: OK
    Luz: OK
    Temp: OK
    Press: OK
    ```  

  - Ao apertar o Botão A Atualiza  a tela do **OLED SSD1306** e vai para tela 2, onde mostra os valores dos sensores:  
    ```
    Luz: 115 LX
    Temp: 28.1 C
    Umid: 40.0
    Press: 892.4 HPA
    ```  
 - Ao apertar o Botão B volta para tela 1 do display oled
 - Salva dados no cacrtão SD
---


## 📝 Observações Importantes

- Certifique-se de que as bibliotecas estejam corretamente clonadas na raiz conforme indicado acima.

## 📚 Referências e Créditos

- FreeRTOS: https://github.com/FreeRTOS/FreeRTOS-Kernel
- no-OS-FatFS-SD-SPI-RPi-Pico: https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico

Projeto desenvolvido por Rafael Florentino Barbosa - Residência Tecnológica em Sistemas Embarcados – Instituto Hardware BR-DF, 2025.