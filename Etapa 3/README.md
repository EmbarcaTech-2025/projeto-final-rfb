# 🌾 ** Estação Agroclimática Portátil**  


## 🎯 **Etapa 3 - Prototipagem e Ajustes **  

**Institution**: Instittuto Hardware BR-DF  
**Course**: Residência tecnologica em sistemas embarcados
**Authors**: **Rafael Florentino Barbosa**  
**Location**: Brasília-DF  
**Date**: Julho de 2025  

---

## 🎥 Fotos e Vídeos ##

Link Video: <video controls src="video_rafael_etapa3.mp4" title="Title"></video>

Fotos:

- Estação Agroclimática Portátil em mdf

![alt text](Protótipo.jpeg)

- Tela 1 do diplay OLED

![alt text](Tela1.jpeg)

- Tela 2 do diplay OLED

![alt text](Tela2.jpeg)

-  Exemplo de Dashboard caso o modulo Wifi seja implementado no futuro 

![alt text](graficos.png)


## Desafios Encontrados Durante o Desenvolvimento ##
Durante o desenvolvimento do protótipo da Estação Agroclimática Portátil, vários desafios foram enfrentados, principalmente relacionados à montagem física dos componentes e à integração dos diferentes sensores. A limitação de espaço no invólucro exigiu uma organização cuidadosa tanto do posicionamento das placas e módulos quanto do roteamento dos cabos para evitar interferências e garantir robustez elétrica. Outro ponto crítico foi o ajuste da comunicação entre os módulos, especialmente na configuração dos barramentos I2C e SPI, onde pequenas incompatibilidades de ligação ou endereçamento poderiam comprometer a leitura correta dos dados. Além disso, surgiram dificuldades na implementação do armazenamento seguro dos dados no cartão SD, exigindo refino do software para lidar com falhas de escrita e detecção do cartão.

## Melhorias Planejadas ##
Entre as melhorias planejadas para as próximas versões do projeto está a utilização de uma placa PCB dedicada, o que ajudará a reduzir o excesso de fios, proporcionar maior confiabilidade das conexões e facilitar a montagem e manutenção do dispositivo. Está prevista também a implementação do módulo Wi-Fi para transmissão remota dos dados em tempo real, ampliando o potencial de monitoramento e automação agrícola. Outras melhorias incluem a adoção de um gabinete mais robusto e com melhor vedação para uso em ambientes externos, a otimização do consumo energético para aumentar a autonomia e a expansão do sistema de interface do usuário, tornando o display OLED ainda mais intuitivo e informativo.

- Inserir o sensor de solo A

![alt text](capacitivo.jpg)

- Inserir o sensor de solo B

![alt text](solo_umidade.jpg)

- Inserir o sensor de chuva

![alt text](sensor_chuva.jpg)