 O programa vai descodificar os datos Wiegand dun RFID HID Reader http://bit.ly/29yz3lj
  * ou, en teor�a,   * Calquera outro dispositivo que xera Weigand de datos.
  * A interface Wiegand ten d�as li�as de datos, DATA0 e DATA1. Estas li�as son normalmente altas
  * Alta non 5V. Cando un 0 � enviado, DATA0 cae 5V. Cando un 1 � enviado, DATA1 cae 0V
  * Hai xeralmente uns poucos ms entre os pulsos.
  *
  * (DATA0) para Dixital Pin 2 (INT0).
  * (DATA1) para Dixital Pin 3 (INT1). Iso e todo!
  *
  * A operaci�n � simple - cada unha das li�as de datos son conectadas �s li�as de interrupci�n de hardware. cando
  * baixa a cero, unha rutina de interrupci�n  alg�ns bits son invertidos. Tras alg�n tempo de
  * De non recibir bits, o Arduino pode decodificar os datos.
  * HID RFID Reader Wiegand Interface para Arduino Uno
  * Escrito por Daniel Smith, 2012/01/30
  * www.pagemac.com
