//variables globales
int contador = 0;
int limite = 3;
char entrada = "";

void setup() {
  Serial.begin(115200);
  menu();
}

void loop() {
  prompt();
  entrada_de_teclado(); //Llama a una función
  seleccion();


  switch (entrada) {
    case '0': contador++; menu(); break;
    case '1': condicion1(); break;
    case '2': condicion2(); break;
    case '3': condicion3(); break;
    case '4': condicion4(); break;
    case '5': condicion5(); break;
    case 'l': valor(); break;
    case 'b': borrar(); menu(); break;
    case 'c': conteo(); break;
    case 'p': limpiarpantalla(); break;
    case '\n': break;
    case '\r': break;
    default: error(); break;
  }
}


void basico() {

  //////////////////Condicional if else//////////////////////
  int numero = 0;
  if (numero < 23) {
    //Bloque de código si la condición es verdadera
  }
  else {
    //Bloque de código si la condición es falsa
  }

  //////////////////Condicional else if////////////////////
  int cantidad = 0;
  if (cantidad < 23) {
    //Bloque de código si la condición es verdadera
  }
  else if (cantidad >= 23) {
    //Bloque de código si la condición es verdadera
  }
  else {
    //Bloque de código si la condición es falsa
  }

  //////////////////Condicional for///////////////////////
  int cuenta = 5;
  for (cuenta; cuenta <= 100; cuenta++) {
    //Bloque de código
  }

  //////////////////Condicional while/////////////////////
  boolean condicion = true;
  while (condicion) {
    //Bloque de código
  }
}

void menu() {
  Serial.print(F("Contador = "));
  Serial.println(contador);
  Serial.print(F("limite = "));
  Serial.println(limite);
  Serial.println("-------------------------------------");
  Serial.println(F("[0] Menú"));
  Serial.println(F("[1] Condiciones - if else"));
  Serial.println(F("[2] Condiciones - else if"));
  Serial.println(F("[3] Condición - for"));
  Serial.println(F("[4] Condición - while "));
  Serial.println(F("[5] Condición - do while "));
  Serial.println(F("[b] Borrar contador"));
  Serial.println(F("[c] Valor de contador"));
  Serial.println(F("[p] Limpiar pantalla"));
  Serial.println(F("[l] Cambiar limite"));
  Serial.println("-------------------------------------");
  Serial.println(F(""));
}

void prompt() {
  if (entrada == '\n') { }
  else if (entrada == '\r') { }
  else {
    Serial.print("> ");
  }
}

void seleccion() {
  if (entrada == '\n') { }
  else if (entrada == '\r') { }
  else {
    Serial.println(entrada);
    Serial.println("");
  }
}

void entrada_de_teclado() {
  while (!Serial.available()) {}
  entrada = Serial.read();
  Serial.flush();
}

void condicion1() {
  Serial.println(F("cont __ lim"));
  Serial.println(F(""));
  contador++; //Operación de sumar una unidad (+1) a la variable contador

  Serial.print(contador); Serial.print(F(" < ")); Serial.print(limite); Serial.print(F(": "));
  if (contador < limite) {
    Serial.println(F("True"));
  }
  else {
    Serial.println(F("False"));
  }

  Serial.print(contador); Serial.print(F(" > ")); Serial.print(limite); Serial.print(F(": "));
  if (contador > limite) {
    Serial.println(F("True"));
  }
  else {
    Serial.println(F("False"));
  }

  Serial.print(contador); Serial.print(F(" <= ")); Serial.print(limite); Serial.print(F(": "));
  if (contador <= limite) {
    Serial.println(F("True"));
  }
  else {
    Serial.println(F("False"));
  }

  Serial.print(contador); Serial.print(F(" >= ")); Serial.print(limite); Serial.print(F(": "));
  if (contador >= limite) {
    Serial.println(F("True"));
  }
  else {
    Serial.println(F("False"));
  }

  Serial.print(contador); Serial.print(F(" == ")); Serial.print(limite); Serial.print(F(": "));
  if (contador == limite) {
    Serial.println(F("True"));
    game();
  }
  else {
    Serial.println(F("False"));
  }

  Serial.print(contador); Serial.print(F(" != ")); Serial.print(limite); Serial.print(F(": "));
  if (contador != limite) {
    Serial.println(F("True"));
  }
  else {
    Serial.println(F("False"));
  }

  Serial.println(F(""));
}

void condicion2() {

  contador++; //Operación de sumar una unidad (+1) a la variable contador

  Serial.print(F("if (")); Serial.print(contador); Serial.print(F(" <= ")); Serial.print(limite); Serial.println(F("){ }"));
  Serial.print(F("else if (")); Serial.print(contador); Serial.print(F(" >= ")); Serial.print(limite); Serial.println(F("){ }"));
  Serial.println(F(""));

  if (contador <= limite) {
    Serial.println(F("Menor o igual"));
  }
  else if (contador >= limite) {
    game();
    Serial.println(F("Mayor o igual"));
  }
  else {
    Serial.println(F("NULL"));
  }
  Serial.println(F(""));
}

void condicion3() {

  contador++; //Operación de sumar una unidad (+1) a la variable contador

  for (int n = 1; n <= limite; n++ ) {
    Serial.print("n = ");
    Serial.println(n);
  }
  game();

  contador = limite;
  Serial.println(F(""));
}

void condicion4() {

  if (contador == 0) {
    Serial.println(F("Condición: while"));
    Serial.println(F(""));
  }

  contador++; //Operación de sumar una unidad (+1) a la variable contador

  int n = -1;
  while (n >= limite) {
    Serial.print("n = ");
    Serial.println(n);
    n--;
  }
  game();
  Serial.println(F(""));
}

void condicion5() {

  contador++; //Operación de sumar una unidad (+1) a la variable contador

  int n = 0;
  do  {
    Serial.print("n = ");
    Serial.println(n);
    n--;
    game();
  } while (n >= limite);

  Serial.println(F(""));
}

void valor() {

  /* ----int----- es un tipo de dato de 16 bits con el MSB representa su signo
     Solamente puede representar número entre -32768 y 32767
     En binario:
      32767 = 0111 1111 1111 1111
     -32768 = 1000 0000 0000 0000
    ......MSB-^.................^-LSB
  */

  Serial.println(F("Digite un número entre -32768 y 32767"));


  boolean espera_dato = true;
  boolean listo = false;
  boolean hmm = false;
  char datos[6] = " ";
  int n = 0;

  while (espera_dato) {

    entrada_de_teclado();

    if ((isDigit(entrada) && entrada != '\n' && entrada != '\r') || (entrada == '-')) {
      datos [n] = entrada;
      n++;
      limite = atoi(datos);
      listo = true;
    }

    if (!isDigit(entrada) && entrada != '\n' && entrada != '\r' && entrada != '-' && hmm == false) {
      Serial.println(F("ヽ(ಠ_ಠ)ノ"));
      hmm = true;
    }

    if (entrada == '\r' && listo == true) {
      espera_dato = false;
    }
  }

  Serial.print("limite: ");
  Serial.println(limite);
  Serial.println(" ");
  Serial.print("> ");

  for ( int i = 0; i < 5;  ++i ) {
    datos[i] = " ";
  }
}

void borrar() {
  Serial.println(F("Borrando..."));
  contador = 0;
}

void limpiarpantalla() {
  for (int n; n <= 50; n++) {
    Serial.println(" ");
  }
}

void conteo() {
  Serial.print(F("Contador = "));
  Serial.println(contador);
}

void error() {
  Serial.println("(✖╭╮✖)");
  Serial.println("");
}

boolean m, c1, c2, c3, c4, c5, b, c, p, l = false;

//l15 3 l16 1
//l-9 3 2
//l46 b 0 3
//l7 3 l-521 4
//l-4 3 l28 5


void game() {
  if ((entrada == '1') && (contador == 0x10)) {
    //Serial.println("c1");
    c1 = true;
  }

  if ((entrada == '2') && (contador == -8)) {
    //Serial.println("c2");
    c2 = true;
  }

  if ((entrada == '3') && (limite == 46) && (contador & B10)) {
    //Serial.println("c3");
    c3 = true;
  }

  if ((entrada == '4') && (limite == -0x00000209) && (contador == 010)) {
    //Serial.println("c4");
    c4 = true;
  }

  if ((entrada == '5') && (limite == 034) && (contador == -B11)) {
    //Serial.println("c5");
    c5 = true;
  }

  if (c1 && c2 && c3 && c4 && c5) {
    for (int n = 1; n <= 2000; n++ ) {
      Serial.println("ヾ(￣0￣ )ノ");
      c1, c2, c3, c4, c5 = false;
    }
  }

}



