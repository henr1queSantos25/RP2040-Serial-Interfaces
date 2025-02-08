# 🔗 Interfaces de Comunicação Serial com RP2040  

Este repositório contém um exemplo de código para a implementação de **interfaces de comunicação serial (UART e I2C)** utilizando o **Raspberry Pi Pico W**. O projeto combina o uso de **LEDs**, **botões físicos**, **display SSD1306** e **uma matriz de LEDs WS2812**.  

## 🎯 Objetivo  

- Compreender o funcionamento e a aplicação de **comunicação serial** em microcontroladores.  
- Aplicar os conhecimentos adquiridos sobre **UART e I2C** na prática.  
- Manipular e controlar **LEDs comuns e LEDs WS2812**.  
- Implementar **interrupções e debounce** para o controle dos botões físicos.  
- Exibir caracteres no **display OLED SSD1306** via **entrada serial (UART)**.  
- Desenvolver um projeto funcional que combine hardware e software.  

## 📹 Vídeo do Projeto  

- **[Ver Vídeo](#)** _(adicione o link do vídeo aqui)_  

## 🛠️ Tecnologias Utilizadas  

- **Linguagem de Programação**: C.  
- **Placa Microcontroladora**: Raspberry Pi Pico W.  
- **Bibliotecas**:  
  - **Pico SDK** para manipulação de GPIO, UART e I2C.  
  - **ssd1306** para controle do display OLED.  
  - **PIO (Programmable I/O)** para controle eficiente da matriz de LEDs WS2812.  
- **Ferramentas**:  
  - **Wokwi** (Simulação).  
  - **VS Code** (Ambiente de Desenvolvimento).  

## 🗂️ Estrutura do Repositório  

- `RP2040SerialInterfaces.c`: Código-fonte principal contendo a implementação das interfaces de comunicação serial.  
- `RP2040SerialInterfaces.pio`: Código PIO para controle da matriz de LEDs.  
- `libs/ssd1306.h` e `libs/font.h`: Bibliotecas para o controle do display OLED SSD1306.  
- `README.md`: Documentação detalhada do projeto.  
- `diagram.json`: Configuração do circuito para simulação no **Wokwi**.  

## 🚀 Como Funciona  

O projeto implementa as seguintes funcionalidades:  

### 📡 **1. Comunicação Serial (UART e I2C)**  
- **Entrada de caracteres via UART** (Serial Monitor do VS Code).  
- **Cada caractere digitado é exibido no display SSD1306**.  
- **Se um número de 0 a 9 for digitado, ele também será mostrado na matriz de LEDs WS2812**.  

### 🔘 **2. Interação com os Botões**  
- **Botão A (GPIO 5):** Alterna o estado do **LED Verde (GP11)** e exibe uma mensagem no **display SSD1306** e no **Serial Monitor**.  
- **Botão B (GPIO 6):** Alterna o estado do **LED Azul (GP12)** e exibe uma mensagem no **display SSD1306** e no **Serial Monitor**.  
- **As ações dos botões são processadas via interrupções (`gpio_irq_handler()`) com debounce.**  

### 🖥️ **3. Uso do Display SSD1306**  
- **Mostra caracteres digitados via UART.**  
- **Exibe mensagens informando o estado dos LEDs quando os botões são pressionados.**  

✅ **Todos os botões utilizam interrupções para melhor desempenho.**  
✅ **O debounce dos botões é tratado via software para evitar múltiplos acionamentos.**  

## 🔌 Configuração do Hardware  

| Componente          | Pino do Pico W |  
|--------------------|---------------|  
| Matriz de LEDs 5x5 WS2812 | GP7  |  
| LED Verde         | GP11          |  
| LED Azul         | GP12          |  
| Botão A (Alterna LED Verde) | GP5  |  
| Botão B (Alterna LED Azul)  | GP6  |  
| Display SSD1306 (I2C) | SDA (GP14) / SCL (GP15) |  

## ✨ Funcionalidades Demonstradas  

- **Uso de UART para entrada de caracteres e envio de mensagens.**  
- **Uso de I2C para comunicação com o display SSD1306.**  
- **Controle da matriz de LEDs WS2812 via PIO.**  
- **Interrupções para captura de eventos dos botões físicos.**  
- **Debounce via software para evitar leituras duplicadas dos botões.**  

## 💡 Desenvolvido por  

- **Henrique Santos**  
- **[LinkedIn](https://www.linkedin.com/in/dev-henriqueo-santos/)**  
