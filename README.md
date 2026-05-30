# Proyecto-Final---Circuitos-2
Versión final del código de mi proyecto de electrónica digital I, el cual consiste en un "Monitor de paciente dual"

Descripción del proyecto
En este proyecto se diseñará e implementará un sistema embebido utilizando un Arduino
Nano que simula el funcionamiento de un monitor biomédico básico.
El sistema contará con tres modos de operación, los cuales permitirán representar
distintas condiciones fisiológicas de un paciente mediante el uso de sensores simulados
(potenciómetros) y actuadores (LED RGB, servomotor, buzzer y display de 7 segmentos).
El cambio de modo se realizará mediante un botón, y el sistema deberá indicar en todo
momento el modo activo utilizando un display de 7 segmentos.

Requisitos generales
- El sistema debe tener 3 modos de operación (0, 1 y 2)
- Debe existir:
Botón 1: cambio de modo
Botón 2: apagado de alarma (Exclusivo Modo 2)
- El display de 7 segmentos debe mostrar constantemente el modo activo
- Todos los componentes deben funcionar de forma integrada (no se calificarán
pruebas aisladas).
- Su código debe contar con al menos una función de diseño propio.

Descripción de funcionamiento de cada Modo de Operación

- Modo 0: Sistema en reposo (suspendido)

Todas las salidas deben estar apagadas:
• LED RGB apagado
• Servomotor en posición 0 grados
• Buzzer apagado
• El display debe mostrar: 0
Las entradas (potenciómetro, botón 2) no deben afectar el sistema, el botón 1
debe cambiar de modo.

- Modo 1: Monitor de Frecuencia Cardíaca

Entradas:
• El potenciómetro debe simular la señal de “frecuencia cardíaca” esta debe
tener valores entre 0 y 200 latidos por minuto (lpm)
• El botón 1 debe cambiar de modo
• El botón 2 no debe afectar al sistema.
Salidas:
• El servomotor debe representar en tiempo real el valor de medición en
donde 0 lpm es equivalente a 0 grados y 200 lpm es equivalente a 180
grados. El movimiento debe de ser continuo.
• El LED RGB debe cambiar de color según el estado en el que se encuentre
el valor de la señal, según los siguientes rangos:
o Menor a 60 lpm → Amarillo (bradicardia)
o Entre 60 y 100 lpm → Verde (normal)
o Mayor a 100 lpm → Rojo (taquicardia)
o Mayor a 150 lpm → Rojo parpadeante (taquicardia severa)
• El display debe mostrar: 1

- Modo 2: Monitor de Temperatura Corporal

Entradas:
• El potenciómetro debe simular la señal de “temperatura corporal” esta
debe tener valores entre 20 y 15 grados centígrados.
• El botón 1 debe cambiar de modo
• El botón 2 debe apagar la alarma (buzzer)
Salidas:
• El servo motor debe colocarse en posiciones definidas (como se muestra
en la siguiente tabla.
• El LED RGB debe cambiar de color según el rango en el que se encuentra
la señal de temperatura (como se especifica en la siguiente tabla).
Rango Condición Color LED RGB Servo:
< 35 Hipotermia Morado 0°
35–37 Normal Cyan (celeste) 45°
37–38 Febrícula Amarillo 90°
38–39 Fiebre Anaranjado 135°
> 39 Fiebre alta Blanco 180°
• Si el rango de la señal se encuentra en Hipotermia o Fiebre alta debe
encenderse una alarma (buzzer) la cual debe permanecer encendida
hasta que se precione el botón 2 o la señal cambie de rango.
• El display debe mostrar: 2.
