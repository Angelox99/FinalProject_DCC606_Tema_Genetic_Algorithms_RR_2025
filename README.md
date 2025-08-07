# 🧠 Projeto Final - Algoritmos Genéticos aplicados ao jogo Snake

Este repositório contém o projeto final da disciplina **DCC606 – Análise de Algoritmos** da Universidade Federal de Roraima, desenvolvido por **Angelo Ferro** e **Kaio Guilherme**. O trabalho explora a aplicação de **Algoritmos Genéticos (AGs)** no treinamento de uma **Rede Neural Artificial (RNA)** para controlar um agente autônomo no ambiente do jogo Snake.

## 🎯 Objetivo
Treinar um agente inteligente que maximize a coleta de alimentos no jogo Snake, utilizando apenas métodos evolutivos, sem aprendizado supervisionado ou reforço clássico.

## ⚙️ Metodologia
- Protótipo inicial feito em Python com a biblioteca **DEAP**, usado apenas para validação conceitual
- Implementação final desenvolvida em **C++**, com foco em desempenho e visualização
- Rede Neural Feedforward representada por vetor de DNA com 42 parâmetros
- Algoritmo Genético com:
  - População de até 20.000 indivíduos
  - 200 gerações
  - Seleção elitista, crossover e mutação
- Fitness baseada na eficiência: maçãs coletadas e passos dados

## 🧪 Resultados
- Evolução de estratégias eficientes ao longo das gerações
- Redução de comportamentos subótimos (como movimentação infinita) com ajustes no ambiente e nas penalizações
- Visualização completa do agente em tempo real, incluindo sensores, ações, estrutura da rede neural e desempenho

## 🖥️ Visualização
A simulação foi renderizada com a biblioteca **Raylib**, oferecendo diagnóstico visual das decisões do agente e facilitando a análise dos comportamentos aprendidos.

## 📁 Estrutura do Repositório
- `snake_ga_cpp/`: Implementação final em C++ com visualização Raylib  
- `Relatorio - Algoritmos Genéticos.pdf`: Documento técnico completo do projeto  
- `slides_apresentacao/`: Apresentação do projeto em slides  
- `referencias/`: Arquivos de apoio e fontes bibliográficas utilizadas  

## 📌 Palavras-chave
`Algoritmos Genéticos`, `Otimização`, `Inteligência Artificial Evolutiva`, `Snake Game`, `C++`, `Rede Neural`, `Agente Autônomo`
