 O programa vai descodificar os datos Wiegand dun RFID HID Reader http://bit.ly/29yz3lj
  * ou, en teoría,   * Calquera outro dispositivo que xera Weigand de datos.
  * A interface Wiegand ten dúas liñas de datos, DATA0 e DATA1. Estas liñas son normalmente altas
  * Alta non 5V. Cando un 0 é enviado, DATA0 cae 5V. Cando un 1 é enviado, DATA1 cae 0V
  * Hai xeralmente uns poucos ms entre os pulsos.
  *
  * (DATA0) para Dixital Pin 2 (INT0).
  * (DATA1) para Dixital Pin 3 (INT1). Iso e todo!
  *
  * A operación é simple - cada unha das liñas de datos son conectadas ás liñas de interrupción de hardware. cando
  * baixa a cero, unha rutina de interrupción  algúns bits son invertidos. Tras algún tempo de
  * De non recibir bits, o Arduino pode decodificar os datos.
  * HID RFID Reader Wiegand Interface para Arduino Uno
  * Escrito por Daniel Smith, 2012/01/30
  * www.pagemac.com
