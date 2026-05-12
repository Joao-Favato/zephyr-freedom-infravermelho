# Zephyr-Freedom-Infravermelho

## 1. Teste do sensor infravermelho
Antes de implementar os sensores no conjunto do carro, primeiramente fiz testes com os sensores utilizando os leds da placa para certificar a funcionalidade deles. Dessa maneira, fiz o teste abaixo com superfícies pretas e de outras cores.

<img width="204" height="366" alt="TesteDosSensoresComLED" src="evidencias/TesteDosSensoresComLED.gif" />


## 2. Implementação de 1 sensor no carro
Tendo testado os sensores infravermelho, testei novamente os movimentos dos motores. Depois de devido atestado o funcionamento de todos os componentes, implementei um único sensor na frente do carrinho. Nessa nova configuração, o carrinho liga os seus dois motores enquanto estiver passando sobre uma superfície preta, como mostrado no vídeo.

<img width="204" height="366" alt="CarroCom1Sensor" src="evidencias\CarroCom1Sensor.gif" />


## 3. Robô seguidor de linha
Depois de conseguir fazer a implementação do segundo sensor, fui fazendo diversos ajustes nos níveis de velocidade dependendo do input capturado. A versão atual, mais próxima do projeto final do robô seguidor de linha, usa um "start" para fazer os motores girarem uma primeira vez e depois mantém uma velocidade mais baixa para permitir a realização das curvas com maior estabilidade. 

<img width="204" height="366" alt="CarroCom1Sensor" src="evidencias/RobôSeguidorDeLinha.gif" />

## 4. Robô seguidor de linha (Versão Final) 
Após a implementação da função de ultrassom por meio de biblioteca no projeto, foram feitos diversos testes para garantir o funcionamento correto do carrinho em diferentes tipos de curvas. A versão final conclui o percurso de maneira consistente, além de conseguir parar a 20 cm de anteparos com uma margem de erro de no máximo 5 milímetros.

<img width="204" height="366" alt="CarroSeguidorDeLinha" src="https://github.com/user-attachments/assets/e46f5a02-aaf4-4560-a812-2dc4231261e9" />


<img width="204" height="366" alt="SensorUltrassom" src="https://github.com/user-attachments/assets/728fe44e-e570-4437-81b2-01c10a7a1407" />
