  #include <Arduino.h> // incluir biblioteca
#include <ESP8266WiFi.h> // incluir biblioteca
#include <NTPClient.h> // incluir biblioteca
#include <WiFiUdp.h> // incluir biblioteca
#ifdef ESP32 // direstiva de identificação de undefined <<Se o ESP for o 32 então>>
  #include <WiFi.h> // incluir biblioteca
  #include <AsyncTCP.h> // incluir biblioteca
#else // <<Senão, ESP é 8266>>
  #include <ESP8266WiFi.h> // incluir biblioteca
  #include <ESPAsyncTCP.h> // incluir biblioteca
#endif // <<Fim da condição>>
#include <ESPAsyncWebServer.h> // incluir biblioteca
#include <Stepper.h> // incluir biblioteca

#define IN1 14 // Define uma constante, incluindo o nome e o valor
#define IN2 12 // Define uma constante, incluindo o nome e o valor
#define IN3 13 // Define uma constante, incluindo o nome e o valor
#define IN4 15 // Define uma constante, incluindo o nome e o valor

#define buzzer 2 // Define uma constante, incluindo o nome e o valor

AsyncWebServer server(80); // criar um objeto AsyncWebServer na porta 80

const char* ssid = "nome wifi"; // nome do wifi
const char* password = "senha do wifi"; // senha do wifi

const char* PARAM_INPUT_1 = "input1"; // entrada de dados do HTML
const char* PARAM_INPUT_2 = "input2"; // entrada de dados do HTML

const char* PARAM_INPUT_3 = "input3"; // entrada de dados do HTML
const char* PARAM_INPUT_4 = "input4";// entrada de dados do HTML

const char* PARAM_INPUT_5 = "input5"; // entrada de dados do HTML
const char* PARAM_INPUT_6 = "input6"; // entrada de dados do HTML


const char index_html[] PROGMEM = R"rawliteral( // Cria uma variavel, para agir como página web
// <<Código cria 2 botões o "User" e a "Senha", se as informações baterem o usuário acessa a página de controle de horários>>
<!DOCTYPE html>
<html>
    <head>
        <title>Projeto TCC Fecap</title>
        <link rel="preconnect" href="https://fonts.googleapis.com">
        <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
        <link href="https://fonts.googleapis.com/css2?family=Lato&display=swap" rel="stylesheet">

    <style>
        *{
    font-family: 'Lato', sans-serif;
}

body{
    background-color:hsl(123, 53%, 28%);
}

.login-div{
    margin: 100px auto;
    width: 350px;
    background-color: rgb(255, 255, 255);
    padding: 45px;
    border-radius: 10%;
}

.logo{
    width: 130px;
    height: 130px;
    border-radius: 50%;
    margin:0 auto;
    margin-bottom: 4px;
}

.logo img {
    width: 100%;
}

.tittle, .sub-tittle{
 text-align: center;
 color:#505050;
}

.tittle{
    font-weight:bolder;
    font-size: 25px;
}

.sub-tittle{
    font-size: 15px;
}

.form{
    width: 100%;
    margin-top: 30px;
}

.form input{
    font-size: 15px;
    padding: 10px 20px 10px 5px;
    border: none;
    outline: none;
    background: none;
}

.username, .senha{
    display: block;
    border-radius: 30px;
    border: 1px solid rgba(0,0,0,0.2);
    padding: 10px;
    margin: 10px 0;
}

.form svg{
    height: 20px;
    margin-bottom: -5px;
    margin-left: 10px;
    margin-right: 5px;
}


.opcoes {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    padding: 5px 0px;
    margin-bottom: 25px;
}

button.Logar-se{
    width: 100%;
    padding: 12px 10px;
    border: none;
    font-size: 15px;
    border-radius: 30px;
    background-color:hsl(123, 53%, 28%) ;
    color: white;
    margin-bottom: 5px;
}


button.Logar-se:hover{
    background-color:hsl(123, 35%, 24%) ;
}

        </style>
 <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body>
    <div class="login-div">
        <div class="logo">   
            <img src="data:image/jpeg;base64,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" alt="">
          </div>
          <div class="tittle">Projeto TCC Fecap</div>
        <div class="sub-tittle">Login Admin</div>
        <div class="form">
        <div class="username">
          <form id = "form_id">
            <svg class="svg-icon" viewBox="0 0 20 20">
                <path d="M12.075,10.812c1.358-0.853,2.242-2.507,2.242-4.037c0-2.181-1.795-4.618-4.198-4.618S5.921,4.594,5.921,6.775c0,1.53,0.884,3.185,2.242,4.037c-3.222,0.865-5.6,3.807-5.6,7.298c0,0.23,0.189,0.42,0.42,0.42h14.273c0.23,0,0.42-0.189,0.42-0.42C17.676,14.619,15.297,11.677,12.075,10.812 M6.761,6.775c0-2.162,1.773-3.778,3.358-3.778s3.359,1.616,3.359,3.778c0,2.162-1.774,3.778-3.359,3.778S6.761,8.937,6.761,6.775 M3.415,17.69c0.218-3.51,3.142-6.297,6.704-6.297c3.562,0,6.486,2.787,6.705,6.297H3.415z"></path>
            </svg>
               <input type="text" placeholder="Username" name = "input1" id = "USERdata">
           
        </div>
        <div class="senha">
            <svg class="svg-icon" viewBox="0 0 20 20">
                <path d="M17.308,7.564h-1.993c0-2.929-2.385-5.314-5.314-5.314S4.686,4.635,4.686,7.564H2.693c-0.244,0-0.443,0.2-0.443,0.443v9.3c0,0.243,0.199,0.442,0.443,0.442h14.615c0.243,0,0.442-0.199,0.442-0.442v-9.3C17.75,7.764,17.551,7.564,17.308,7.564 M10,3.136c2.442,0,4.43,1.986,4.43,4.428H5.571C5.571,5.122,7.558,3.136,10,3.136 M16.865,16.864H3.136V8.45h13.729V16.864z M10,10.664c-0.854,0-1.55,0.696-1.55,1.551c0,0.699,0.467,1.292,1.107,1.485v0.95c0,0.243,0.2,0.442,0.443,0.442s0.443-0.199,0.443-0.442V13.7c0.64-0.193,1.106-0.786,1.106-1.485C11.55,11.36,10.854,10.664,10,10.664 M10,12.878c-0.366,0-0.664-0.298-0.664-0.663c0-0.366,0.298-0.665,0.664-0.665c0.365,0,0.664,0.299,0.664,0.665C10.664,12.58,10.365,12.878,10,12.878"></path>
            </svg>
            <input type="password" placeholder="Senha" name = "input2" id = "SENHAdata">
        </div>
         <div class="opcoes">
         </div>
                <button type = "submit" onclick="Login()" class="Logar-se">Logar</button>

                <script>
                const Login = () =>{
               USER = document.getElementById("USERdata").value; 
               SENHA = document.getElementById("SENHAdata").value; 
               if (USER == "Gustavo" && SENHA == "guga3108" || USER == "TestUser" && SENHA == "TestSenha"){
               alert("Usuario correto!")
               document.getElementById("form_id").action = "/get"
               document.getElementById("form_id").submit();
               }
               else{
                   alert("Usuario ou Senha incorretos!")
               }
    }
                </script>
           </form>
    </div>
 </div>
    </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) { // Se a rede não for encontrada

  request->send(404, "text/plain", "Not found"); // Error de Rede 404!
}

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org"); // Solicitaremos o horário de pool.ntp.org

int currentHour;// define uma variável do tipo inteira

int currentMinute; // define uma variável do tipo inteira

String SaveUSER; // define uma variável do tipo String

String SaveSENHA; // define uma variável do tipo String

int H1; // define uma variável do tipo inteira
int H2; // define uma variável do tipo inteira
int H3; // define uma variável do tipo inteira
int H4; // define uma variável do tipo inteira

String SaveH1; // define uma variável do tipo String
String SaveH2; // define uma variável do tipo String
String SaveH3; // define uma variável do tipo String
String SaveH4; // define uma variável do tipo String

bool Debounce = true; // define uma variável do tipo boolena

bool Turned = true; // define uma variável do tipo boolena

const int stepsPerRevolution = 512; // define uma constante do tipo inteira

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4); // Inicialize uma instância da biblioteca AccelStepper chamada stepper. Controlande do motor de passo com quatro fios e os pinos de entrada. A ordem dos pinos é IN1, IN3, IN2, IN4

void setup() {
  Serial.begin(115200); // define a taxa de comunicação de bits por segundo
   pinMode(buzzer,OUTPUT); // define o pino do "buzzer" como OUTPUT
    myStepper.setSpeed(500); // Define a velocidade em passos por segundo.
  WiFi.mode(WIFI_STA); // define o modo de conexão como "WIFI_STA"
  WiFi.begin(ssid, password); // iniciar a conexão com nome e senha da rede
  if (WiFi.waitForConnectResult() != WL_CONNECTED) { // se a conexão com o Wi.Fi falhar
    Serial.println("WiFi Failed!"); // printa na tela a mensagem <<Printa "Wifi Failed!"
    return; // retorna a condição da variável
  }
    Serial.println(); // printa na tela
    Serial.print("IP Address: ");// printa na tela
    Serial.println(WiFi.localIP());// printa na tela

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ // <<O nome da página é exatamente o primeiro parâmetro, por exemplo: nessa linha o nome da página é “/”>>
    request->send_P(200, "text/html", index_html); // Executa a String "index_html" como text/html no endereço web 
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) { // <<O nome da página é exatamente o primeiro parâmetro, por exemplo: nessa linha o nome da página é “/get”>>

  String USER;// define uma variável do tipo String
  String SENHA;// define uma variável do tipo String

     USER = request->getParam(PARAM_INPUT_1)->value(); // <<Solicita o parâmetro "PARAM_INPUT1", atribuindo o valor a variável "USER">>

     SENHA = request->getParam(PARAM_INPUT_2)->value(); // <<Solicita o parâmetro "PARAM_INPUT2", atribuindo o valor a variável "SENHA">>

SaveUSER = USER; // <<Atribuí o valor da variável "User" na variável "SaveUSER">>

SaveSENHA = SENHA; // <<Atribuí o valor da variável "SENHA" na variável "SaveSenha">>
 
  Serial.println("Usuario: " + USER); // printa na tela a mensagem 
  Serial.println("Senha: " + SENHA); // printa na tela a mensagem 
  Serial.println(""); // printa na tela a mensagem 

    request->send(200,"text/html","<!DOCTYPE html><html>" // Executa o código HTML como text/html no endereço web 
// <<Código cria 4 botões que recebem as variaveis "H1", "H2", "H3" e "H4", Onde os valores podem ser alterados e enviados para o ESP. Cria uma divisão que mostra o nome do "USER">>
"<head>"
"<link rel='preconnect' href='https://fonts.googleapis.com'>"
"<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>" 
"<link href='https://fonts.googleapis.com/css2?family=Lato&display=swap' rel='stylesheet'>" 
"<title>Projeto TCC Fecap</title>"
"<style>"
  "  *{"
  "  font-family: 'Lato', sans-serif;"
"}"
"body{"
   " background-color:hsl(123, 53%, 28%);"
"}"
".svg-icon{"
  "  height: 20px;"
  "  margin-bottom: -5px;"
  "  margin-left: 10px;"
  "  margin-right: 5px;"
"}"
".Time{ "
  "  display: block;"
  "  border-radius: 30px;"
  "  border: 1px solid rgba(0,0,0,0.2);"
  "  padding: 10px;"
  "  margin: 10px 0;"
"}"
".user{"
  "  background-color:white;"
  "  display: block;"
  "  border-radius: 30px;"
  "  border: 1px solid rgba(0,0,0,0.2);"
  "  padding: 5px;"
  "  position: relative;"
  "  width: 200px;"
  "  height: 20px;"
  "  margin: 10px 0;"
"}"
".user input{"
 "   font-size: 15px;"
 "   padding: 10px 20px 10px 5px;"
 "   border: none;"
 "   outline: none;"
 "   background: none;"
"}"
".BackSub{"
 "   margin: -50px auto;"
 "   position:fixed;"
 "   width: 350px;"
 "   background-color: rgb(255, 255, 255);"
 "  padding: 45px;"
 "   border-radius: 10%;"
"}"
".forms{"
 "   text-align: center;"
 "   margin: 100px auto;"
 "   width: 450px;"
 "   padding: 20px;"
 "   border-radius: 10%;"
"}"
".tittle, .sub-tittle{"
 "text-align: center;"
 "color:#505050;"
"}"
".tittle{"
 "   font-weight:bolder;"
 "   font-size: 25px;"
"}"
"button.Enviar{"
  "  width: 100%;"
  "  padding: 12px 10px;"
  "  border: none;"
  "  font-size: 15px;"
  "  border-radius: 30px;"
  "  background-color:hsl(123, 53%, 28%) ;"
 "   color: white;"
 "   margin-bottom: 5px;"
"}"
"button.Enviar:hover{"
"    background-color:hsl(123, 35%, 24%) ;"
"}"
"</style>"
"</head>"
"<body>"
        "<div class='user'>"
       "     <svg class='svg-icon' viewBox='0 0 20 20'>"
      "          <path d='M12.075,10.812c1.358-0.853,2.242-2.507,2.242-4.037c0-2.181-1.795-4.618-4.198-4.618S5.921,4.594,5.921,6.775c0,1.53,0.884,3.185,2.242,4.037c-3.222,0.865-5.6,3.807-5.6,7.298c0,0.23,0.189,0.42,0.42,0.42h14.273c0.23,0,0.42-0.189,0.42-0.42C17.676,14.619,15.297,11.677,12.075,10.812 M6.761,6.775c0-2.162,1.773-3.778,3.358-3.778s3.359,1.616,3.359,3.778c0,2.162-1.774,3.778-3.359,3.778S6.761,8.937,6.761,6.775 M3.415,17.69c0.218-3.51,3.142-6.297,6.704-6.297c3.562,0,6.486,2.787,6.705,6.297H3.415z'></path>"
     "       </svg> Seu user &colon; "+SaveUSER+""
    "    </div>"
   "     <div class='forms'>"
  "          <div class='BackSub'>"
"<form action = /set>"
   " <div class='tittle'>Projeto TCC Fecap</div>"
   " <div class='sub-tittle'>Controle dos Horarios</div>"
   " <p></p>"
   " <center>"
   " <svg class='svg-icon' viewBox='0 0 20 20'>"
   "     <path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
  "  </svg>"
"1&#186 Horario  <input type='time' name = 'input3' class='Time' value = '"+SaveH1+"'>"
"<p></p>"
"<svg class='svg-icon' viewBox='0 0 20 20'>"
 "   <path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
"</svg>"
"2&#186 Horario  <input type='time' name = 'input4' class='Time' value = '"+SaveH2+"'>"
"<p></p>"
"<svg class='svg-icon' viewBox='0 0 20 20'>"
  "  <path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
"</svg>"
"3&#186 Horario  <input type='time' name = 'input5' class='Time' value = '"+SaveH3+"'>"
"<p></p>"
"<svg class='svg-icon' viewBox='0 0 20 20'>"
    "<path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
"</svg>"
"4&#186 Horario  <input type='time' name = 'input6' class='Time' value = '"+SaveH4+"'>"
"<p></p>"
"</center>"
"<button value='Submit' class='Enviar'>Enviar</button>"
"</form>"
"</div>"
 "</div>"
  "</div>"
   "</div>"
    "</body>"
     "</html>");
  });
  
  server.on("/set", HTTP_GET, [] (AsyncWebServerRequest *request) { // <<O nome da página é exatamente o primeiro parâmetro, por exemplo: nessa linha o nome da página é “/set”>>

 String PARAM_3 = request->getParam(PARAM_INPUT_3)->value();// solicita um valor do html, salvando ele na String
    String PARAM_4 = request->getParam(PARAM_INPUT_4)->value();// solicita um valor do html, salvando ele na String
    String PARAM_5 = request->getParam(PARAM_INPUT_5)->value();// solicita um valor do html, salvando ele na String
    String PARAM_6 = request->getParam(PARAM_INPUT_6)->value();// solicita um valor do html, salvando ele na String

    String PARAM_3_HOR = PARAM_3.substring(0, 2);// retorna a parte da string entre os índices inicial e final
     String PARAM_3_MIN = PARAM_3.substring(3, 5);// retorna a parte da string entre os índices inicial e final
    String PARAM_4_HOR = PARAM_4.substring(0, 2);// retorna a parte da string entre os índices inicial e final
    String PARAM_4_MIN = PARAM_4.substring(3, 5);// retorna a parte da string entre os índices inicial e final
    String PARAM_5_HOR = PARAM_5.substring(0, 2);// retorna a parte da string entre os índices inicial e final
    String PARAM_5_MIN = PARAM_5.substring(3, 5);// retorna a parte da string entre os índices inicial e final
    String PARAM_6_HOR = PARAM_6.substring(0, 2);// retorna a parte da string entre os índices inicial e final
    String PARAM_6_MIN = PARAM_6.substring(3, 5);// retorna a parte da string entre os índices inicial e final

    PARAM_3 = PARAM_3_HOR + PARAM_3_MIN;// salva os dois índice em uma variável
    PARAM_4 = PARAM_4_HOR + PARAM_4_MIN;// salva os dois índice em uma variável
    PARAM_5 = PARAM_5_HOR + PARAM_5_MIN;// salva os dois índice em uma variável
    PARAM_6 = PARAM_6_HOR + PARAM_6_MIN;// salva os dois índice em uma variável

    H1 = PARAM_3.toInt();// transforma em inteiro
    H2 = PARAM_4.toInt();// transforma em inteiro
    H3 = PARAM_5.toInt();// transforma em inteiro
    H4 = PARAM_6.toInt();// transforma em inteiro

    SaveH1 = PARAM_3_HOR + ":" + PARAM_3_MIN;// salva os dois índice em uma variável
    SaveH2 = PARAM_4_HOR + ":" + PARAM_4_MIN;// salva os dois índice em uma variável
    SaveH3 = PARAM_5_HOR + ":" + PARAM_5_MIN;// salva os dois índice em uma variável
    SaveH4 = PARAM_6_HOR + ":" + PARAM_6_MIN;// salva os dois índice em uma variável

    String USER = SaveUSER;// cria uma variável do tipo String

    String SENHA = SaveSENHA;// cria uma variável do tipo String

 
      request->send(200,"text/html","<!DOCTYPE html><html>" // Executa o código HTML como text/html no endereço web 
// <<Código cria 4 botões que recebem as variaveis "H1", "H2", "H3" e "H4", Onde os valores podem ser alterados e enviados para o ESP. Cria uma divisão que mostra o nome do "USER">>
"<head>"
"<link rel='preconnect' href='https://fonts.googleapis.com'>"
"<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>" 
"<link href='https://fonts.googleapis.com/css2?family=Lato&display=swap' rel='stylesheet'>" 
"<title>Projeto TCC Fecap</title>"
"<style>"
  "  *{"
  "  font-family: 'Lato', sans-serif;"
"}"
"body{"
   " background-color:hsl(123, 53%, 28%);"
"}"
".svg-icon{"
  "  height: 20px;"
  "  margin-bottom: -5px;"
  "  margin-left: 10px;"
  "  margin-right: 5px;"
"}"
".Time{ "
  "  display: block;"
  "  border-radius: 30px;"
  "  border: 1px solid rgba(0,0,0,0.2);"
  "  padding: 10px;"
  "  margin: 10px 0;"
"}"
".user{"
  "  background-color:white;"
  "  display: block;"
  "  border-radius: 30px;"
  "  border: 1px solid rgba(0,0,0,0.2);"
  "  padding: 5px;"
  "  position: relative;"
  "  width: 200px;"
  "  height: 20px;"
  "  margin: 10px 0;"
"}"
".user input{"
 "   font-size: 15px;"
 "   padding: 10px 20px 10px 5px;"
 "   border: none;"
 "   outline: none;"
 "   background: none;"
"}"
".BackSub{"
 "   margin: -50px auto;"
 "   position:fixed;"
 "   width: 350px;"
 "   background-color: rgb(255, 255, 255);"
 "  padding: 45px;"
 "   border-radius: 10%;"
"}"
".forms{"
 "   text-align: center;"
 "   margin: 100px auto;"
 "   width: 450px;"
 "   padding: 20px;"
 "   border-radius: 10%;"
"}"
".tittle, .sub-tittle{"
 "text-align: center;"
 "color:#505050;"
"}"
".tittle{"
 "   font-weight:bolder;"
 "   font-size: 25px;"
"}"
"button.Enviar{"
  "  width: 100%;"
  "  padding: 12px 10px;"
  "  border: none;"
  "  font-size: 15px;"
  "  border-radius: 30px;"
  "  background-color:hsl(123, 53%, 28%) ;"
 "   color: white;"
 "   margin-bottom: 5px;"
"}"
"button.Enviar:hover{"
"    background-color:hsl(123, 35%, 24%) ;"
"}"
"</style>"
"</head>"
"<body>"
        "<div class='user'>"
       "     <svg class='svg-icon' viewBox='0 0 20 20'>"
      "          <path d='M12.075,10.812c1.358-0.853,2.242-2.507,2.242-4.037c0-2.181-1.795-4.618-4.198-4.618S5.921,4.594,5.921,6.775c0,1.53,0.884,3.185,2.242,4.037c-3.222,0.865-5.6,3.807-5.6,7.298c0,0.23,0.189,0.42,0.42,0.42h14.273c0.23,0,0.42-0.189,0.42-0.42C17.676,14.619,15.297,11.677,12.075,10.812 M6.761,6.775c0-2.162,1.773-3.778,3.358-3.778s3.359,1.616,3.359,3.778c0,2.162-1.774,3.778-3.359,3.778S6.761,8.937,6.761,6.775 M3.415,17.69c0.218-3.51,3.142-6.297,6.704-6.297c3.562,0,6.486,2.787,6.705,6.297H3.415z'></path>"
     "       </svg> Seu user &colon; "+SaveUSER+""
    "    </div>"
   "     <div class='forms'>"
  "          <div class='BackSub'>"
"<form action = /set>"
   " <div class='tittle'>Projeto TCC Fecap</div>"
   " <div class='sub-tittle'>Controle dos Horarios</div>"
   " <p></p>"
   " <center>"
   " <svg class='svg-icon' viewBox='0 0 20 20'>"
   "     <path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
  "  </svg>"
"1&#186 Horario  <input type='time' name = 'input3' class='Time' value = '"+SaveH1+"'>"
"<p></p>"
"<svg class='svg-icon' viewBox='0 0 20 20'>"
 "   <path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
"</svg>"
"2&#186 Horario  <input type='time' name = 'input4' class='Time' value = '"+SaveH2+"'>"
"<p></p>"
"<svg class='svg-icon' viewBox='0 0 20 20'>"
  "  <path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
"</svg>"
"3&#186 Horario  <input type='time' name = 'input5' class='Time' value = '"+SaveH3+"'>"
"<p></p>"
"<svg class='svg-icon' viewBox='0 0 20 20'>"
    "<path d='M10.25,2.375c-4.212,0-7.625,3.413-7.625,7.625s3.413,7.625,7.625,7.625s7.625-3.413,7.625-7.625S14.462,2.375,10.25,2.375M10.651,16.811v-0.403c0-0.221-0.181-0.401-0.401-0.401s-0.401,0.181-0.401,0.401v0.403c-3.443-0.201-6.208-2.966-6.409-6.409h0.404c0.22,0,0.401-0.181,0.401-0.401S4.063,9.599,3.843,9.599H3.439C3.64,6.155,6.405,3.391,9.849,3.19v0.403c0,0.22,0.181,0.401,0.401,0.401s0.401-0.181,0.401-0.401V3.19c3.443,0.201,6.208,2.965,6.409,6.409h-0.404c-0.22,0-0.4,0.181-0.4,0.401s0.181,0.401,0.4,0.401h0.404C16.859,13.845,14.095,16.609,10.651,16.811 M12.662,12.412c-0.156,0.156-0.409,0.159-0.568,0l-2.127-2.129C9.986,10.302,9.849,10.192,9.849,10V5.184c0-0.221,0.181-0.401,0.401-0.401s0.401,0.181,0.401,0.401v4.651l2.011,2.008C12.818,12.001,12.818,12.256,12.662,12.412'></path>"
"</svg>"
"4&#186 Horario  <input type='time' name = 'input6' class='Time' value = '"+SaveH4+"'>"
"<p></p>"
"</center>"
"<button value='Submit' class='Enviar'>Enviar</button>"
"</form>"
"</div>"
 "</div>"
  "</div>"
   "</div>"
    "</body>"
     "</html>");
  });

server.onNotFound(notFound); // se não for encontrado, chama a função notFound
server.begin(); // inicializa o servidor


timeClient.begin();// inicializa a função de tempo
timeClient.setTimeOffset(0); // Define uma data que é combinada com uma hora do dia com base em um relógio de 24 horas
  
}

void loop() {

 timeClient.update(); // vai atualizar o horário

    currentHour = timeClient.getHours(); // traz as horas do cliente

    currentMinute = timeClient.getMinutes(); // traz os minutos do cliente

    int currentSecond = timeClient.getSeconds();// traz os segundos do cliente

    if (currentHour == 3) // adaptaçao ao meridiano de greenwich
    {
        currentHour = 0;// define o horário como zero
    }
    else if (currentHour == 2)// adaptaçao ao meridiano de greenwich
    {
        currentHour = 23; // define o horário como vinte e três 
    }
    else if (currentHour == 1)// adaptaçao ao meridiano de greenwich
    {
        currentHour = 22;// define o horário como vinte e dois
    }
    else if (currentHour == 0)// adaptaçao ao meridiano de greenwich
    {
        currentHour = 21; // define o horário como vinte e um
    }
    else // Senão
    {
        currentHour = currentHour - 3; // define como três horas a menos
    }

    int Time = currentHour * 100 + currentMinute; // define o tempo como a hora vezes cem mais minutos

    if (Time == H1) // <<se time for igual a H1>>
    {

   if (Debounce == true) { // <<se "Debounce" for "true" então>>
    
    Debounce = false; // <<Debounce vai ser falso>>
    
  Serial.println("Porta1"); // printa na tela

digitalWrite(buzzer,HIGH); // <<Ativa a porta "buzzer">>

myStepper.step(stepsPerRevolution); // definir uma posição de destino do Motor

  int Sec = 60 - currentSecond; // define uma variável do tipo inteira
  
delay(Sec*1000); // <<Executa um delay de "Sec x 1000"

digitalWrite(buzzer,LOW); // <<Desativa a porta "buzzer">>

myStepper.step(-stepsPerRevolution); // definir uma posição de destino do Motor <<O contrario de stepsPerRevolution>>

    Debounce = true; // <<Debounce vai ser falso>>
   
  }
}

    if (Time == H2) // <<se time for igual a H2>>
    {

   if (Debounce == true) { // <<se "Debounce" for "true" então>>
    
    Debounce = false; // <<Debounce vai ser falso>>
    
  Serial.println("Porta2"); // printa na tela

digitalWrite(buzzer,HIGH); // <<Ativa a porta "buzzer">>

myStepper.step(stepsPerRevolution * 2); // definir uma posição de destino do Motor <<stepsPerRevolution * 2>>

  int Sec = 60 - currentSecond; // define uma variável do tipo inteira
  
delay(Sec*1000); // <<Executa um delay de "Sec x 1000"

digitalWrite(buzzer,LOW); // <<Desativa a porta "buzzer">>

myStepper.step(-stepsPerRevolution * 2); // definir uma posição de destino do Motor <<O contrario de stepsPerRevolution.StepsPerRevolution * 2>>

    Debounce = true; // <<Debounce vai ser falso>>
   
  }
}
   
    if (Time == H3) // <<se time for igual a H3>>
    {

   if (Debounce == true) { // <<se "Debounce" for "true" então>>
    
    Debounce = false; // <<Debounce vai ser falso>>
    
    Serial.println("Porta3"); // printa na tela

     digitalWrite(buzzer,HIGH); // <<Ativa a porta "buzzer">>

myStepper.step(stepsPerRevolution * 3); // definir uma posição de destino do Motor <<stepsPerRevolution * 3>>

  int Sec = 60 - currentSecond; // define uma variável do tipo inteira
  
delay(Sec*1000); // <<Executa um delay de "Sec x 1000"

digitalWrite(buzzer,LOW); // <<Desativa a porta "buzzer">>

myStepper.step(-stepsPerRevolution * 3); // definir uma posição de destino do Motor <<O contrario de stepsPerRevolution.StepsPerRevolution * 3>>

    Debounce = true; // <<Debounce vai ser falso>>
   
  }
}

 if (Time == H4) // <<se time for igual a H3>>
    {

   if (Debounce == true) { // <<se "Debounce" for "true" então>>
    
    Debounce = false; // <<Debounce vai ser falso>>
    
    Serial.println("Porta4"); // printa na tela

    digitalWrite(buzzer,HIGH); // <<Ativa a porta "buzzer">>

  int Sec = 60 - currentSecond; // define uma variável do tipo inteira
  
delay(Sec*1000); // <<Executa um delay de "Sec x 1000"

digitalWrite(buzzer,LOW); // <<Desativa a porta "buzzer">>

    Debounce = true; // <<Debounce vai ser falso>>
   
  }
}

}
