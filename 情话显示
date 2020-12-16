//主要原理：按键计数器及随机数 
int pushButton=7; 
int buttonState=0; 
int beforeState=0;  
int presstime=0;
void setup() 
{
  Serial.begin(9600);  
  pinMode(pushButton, INPUT);  
}

void loop() 
{
  int n = 7;
  int a = random(0,n-1);
  buttonState=digitalRead(pushButton);  
  if(buttonState==1 and beforeState==0)  
  {
    Serial.print(++presstime);
    if(a%n==0){Serial.println("  朝暮与岁月并往，愿我们一同行至天光");}
    if(a%n==1){Serial.println("  月色与雪色之间，你是第三种绝色");}
    if(a%n==2){Serial.println("  生活美好 无关四季 只关于你");}
    if(a%n==3){Serial.println("  他们有许多爱的道理，我有你");}
    if(a%n==4){Serial.println("  对你说的晚安，都是梦里相见的暗号");}
    if(a%n==5){Serial.println("  你最可爱了。我说的时候来不及思索，我仔细想过之后，还是会这么说");}
    if(a%n==6){Serial.println("  你知道你和星星有什么区别吗？星星在天上，你在我心里");}
    //可继续添加并将n改为语句数量
    
    
   }
   if(buttonState!=beforeState)//消抖
   {
    delay(20);
    }
   beforeState=buttonState;   
   delay(100);
  
}
