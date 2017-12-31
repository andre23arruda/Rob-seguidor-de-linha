int le_botao(int pino, int estado)
{
 /*
 * funcao para mudar de estado se o botao for pressionado
 * INPUTS:
 *  pino: variavel que contem o pino conectado ao botao
 *  estado: 1 = ligado, 0 = desligado
 */
  int atual = digitalRead(x); // leitura do pino
  int aterior = LOW; // iniciando a variavel atual
  if ((anterior == HIGH) && (atual == LOW)) // se o estado anterior for LOW e atual for HIGH (botao foi pressionado e largado)
    estado = 1 - estado; // 1 = ligado, 0 = desligado
  else // senao, atualiza o valor de anterior
    anterior = atual;
  return estado; // retorna o valor de estado
}
