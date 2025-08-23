# 🌱 Projeto: Monitor Ambiental com BitDogLab Etapa2

## 🎯 Objetivo do Projeto  

Desenvolver um sistema embarcado utilizando a **BitDogLab (Raspberry Pi Pico W)** para monitoramento ambiental em tempo real.  
O sistema coleta informações de **umidade do solo (digital e capacitivo), luminosidade (BH1750) e temperatura/umidade do ar (AHT10)**, exibindo os dados tanto no **terminal serial** quanto no **display OLED SSD1306**.  

---

## 🛠️ Problema a Ser Resolvido  

Produtores agrícolas e estudantes de tecnologia necessitam de ferramentas de **baixo custo e portáteis** para monitorar condições ambientais locais.  
Esse sistema fornece informações básicas mas relevantes para **manejo de irrigação, controle de luminosidade e microclima**, podendo servir como base para uma estação IoT no futuro.  

---

## ⚙️ Requisitos do Sistema  

### ✅ Requisitos Funcionais  

- Medir periodicamente:  
  - 🌡️ **Temperatura e Umidade do ar** com o sensor **AHT10**;  
  - 💧 **Umidade do Solo A** com sensor **digital (DO)**;  
  - 💧 **Umidade do Solo B** com sensor **capacitivo (via ADC)**;  
  - ☀️ **Luminosidade ambiente** com o sensor **BH1750**.  
- Exibir os dados em **tempo real** no **OLED SSD1306** em até 4 linhas.  
- Registrar os valores no **terminal serial** para debug e acompanhamento.  

### 🚫 Requisitos Não Funcionais  

- O sistema deve ser **portátil**, alimentado por USB ou power bank.  
- Interface de uso deve ser **simples e direta** (sem menus complexos).  
- Leitura dos sensores deve ser **confiável e com filtragem básica** (ex.: média no ADC do solo capacitivo).  

---

## 📦 Lista de Materiais  

| Componente                  | Função                                        |
|-----------------------------|-----------------------------------------------|
| BitDogLab (RP2040 + Wi-Fi)  | Microcontrolador principal                    |
| Sensor AHT10                | Medição de temperatura e umidade do ar        |
| Sensor BH1750               | Medição de luminosidade ambiente              |
| Sensor de Solo Digital (A)  | Detecção binária de solo úmido/seco           |
| Sensor de Solo Capacitivo (B)| Medição analógica de umidade do solo         |
| Display OLED SSD1306        | Exibição das leituras em tempo real           |
| Protoboard + Jumpers        | Conexão entre sensores e microcontrolador     |
| Fonte USB / Power Bank      | Alimentação portátil                          |

---

## 📂 Estrutura do Código  
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
│ ├── air.c # Leitura AHT10
│ ├── light.c # Leitura BH1750
│ ├── soloA.c # Solo digital
│ ├── soloB.c # Solo capacitivo
│ └── display.c # Funções do display OLED
├── include/
│ ├── air.h
│ ├── light.h
│ ├── soloA.h
│ ├── soloB.h
│ └── display.h
├── CMakeLists.txt
└── README.md
```  
---

## 📊 Funcionamento  

- A cada ciclo (2 segundos):  
  - Lê os sensores de **solo A (digital)**, **solo B (capacitivo, média 30 amostras)**, **luminosidade (BH1750)** e **ar (AHT10)**.  
  - Exibe os valores no terminal serial:  

    ```
    SoloA: Umido | SoloB: 58.0% | Lux: 107.5 | T:25.0C U:43.6%
    ```  

  - Atualiza o **OLED SSD1306** em 4 linhas:  
    ```
    SoloA: Umido
    SoloB: 58.0%
    Lux: 107.5
    T:25.0C U:43.6%
    ```  

---

## 📷 Protótipo

![alt text](prototipo.jpeg)

## 🗓️ Entregável Atual Etapa2

### 🏗️ 1. Arquitetura de Hardware

![alt text](arquitetura.jpg)

- OLED SSD1306 → exibição dos dados em 4 linhas.
- AHT10 → temperatura e umidade do ar via I²C1.
- BH1750 → luminosidade via I²C0.
- Sensor Solo A → umidade do solo digital (seco/úmido) no GPIO16.
- Sensor Solo B → umidade do solo capacitivo via ADC no GPIO28.
- BitDogLab (Pico W) → processamento, integração de sensores e futura conexão Wi-Fi.

### 🧩 2. Blocos Funcionais

![alt text](<Bloco Funcional.jpg>)

### 🔄 3. Fluxograma do Software

![alt text](<parte 1.jpg>)

![alt text](<parte 2.jpg>)


- Código modularizado em **app**, **hal**, **drivers** e **include**.  
- Sistema já funcionando com captura e exibição de dados em **display OLED** e **terminal serial**.  
- Base pronta para futura integração com **Wi-Fi (HTTP/MQTT)** e **armazenamento externo (SD ou nuvem)**.  


---

**Aluno:** Rafael Florentino Barbosa  
**Curso:** EmbarcaTech 2025  
**Instituição:** EmbarcaTech / HBr  
**Data:** 8 de agosto de 2025 