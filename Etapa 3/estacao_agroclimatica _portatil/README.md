
# 🌾 Estação Agroclimática Portátil

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

text

## 📝 Observações Importantes

- Certifique-se de que as bibliotecas estejam corretamente clonadas na raiz conforme indicado acima.
- **Não** versionar o arquivo `credentials.h`, pois ele pode conter informações sensíveis.

## 📚 Referências e Créditos

- FreeRTOS: https://github.com/FreeRTOS/FreeRTOS-Kernel
- no-OS-FatFS-SD-SPI-RPi-Pico: https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico

Projeto desenvolvido por Rafael Florentino Barbosa - Residência Tecnológica em Sistemas Embarcados – Instituto Hardware BR-DF, 2025.