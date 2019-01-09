
class Note{
  public:
  
    friend void validateTest();
    Note(char noteInput);
    Note(char noteInput, int noteVelocity);
    Note(char noteInput, int noteVelocity, int octave);
    Note(char noteInput, char sharpFlat, int noteVelocity, int octave);
    void validateInput();
    void augmentNote();
    void noteOff();
    void noteOn();
    void executeNoteState();
    void changeNoteOctave(int);
    //checks the array slots to see if ID is already taken, if not fill in a 1 to indicate slot is taken for the moment
    static int IDSlotAssignment[16];
    //array full of Note on array Ids
    static const int arrayOnIds[16];
    static const int arrayOffIds[16];

  private:
    int noteState;
    int setOctave;
    int savedOctave;
    int applyOctave;
    int augmentVal;
    char noteInput;
    char sharpFlat;  
    int noteVelocity;
    int memberIDOn;
    int memberIDOff;
    int temporaryIDSlotIndex;    
};





Note::Note(char noteInput):noteState(0),noteInput(noteInput),sharpFlat(0),noteVelocity(100){
  //no octave is applied  
  applyOctave = 0;
  //sharp or flat is set to null value
  augmentVal = 0;
  validateInput();
}

Note::Note(char noteInput, int noteVelocity):noteState(0),noteInput(noteInput),sharpFlat(0),noteVelocity(noteVelocity){   
  applyOctave = 0;
  augmentVal = 0;  
  validateInput();
}

Note::Note(char noteInput, int noteVelocity, int octave):noteState(0),noteInput(noteInput),sharpFlat(sharpFlat),setOctave(octave), noteVelocity(noteVelocity){
  applyOctave = 0;
  augmentVal = 0;
  validateInput();
}

Note::Note(char noteInput, char sharpFlat,int noteVelocity, int octave):noteState(0),noteInput(noteInput),sharpFlat(sharpFlat),setOctave(octave),noteVelocity(noteVelocity){
  applyOctave = 0;
  augmentVal = 0; 
  augmentNote();
  validateInput();
}

void Note::changeNoteOctave(int octave){
  noteInput = savedOctave+(octave*12);
}

int Note::IDSlotAssignment[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//note on IDs
const int Note::arrayOnIds[16] = {144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159};
//corresponding note off IDs
const int Note::arrayOffIds[16] =  {128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143};

//poly id
void Note::noteOn(){  
  if(noteState >= 128 && noteState<= 143  || noteState == 0){
    for(int i = 0; i < 16; i++){
      if(IDSlotAssignment[i] == 0){
        temporaryIDSlotIndex = i; //set index member var
        memberIDOn = arrayOnIds[i];
        memberIDOff = arrayOffIds[i];
        IDSlotAssignment[i]  = 1;
        break;
      } 
      
    }
    Serial.println(memberIDOn);
    noteState = memberIDOn;
    executeNoteState();

  }
}
void Note::noteOff(){
  //note off
  if(noteState >= 144 && noteState < 159)
    //get ready to execute note off by setting noteState to the off value
    noteState = memberIDOff;
    executeNoteState();
    noteState = 0;
    memberIDOn = 0;
    memberIDOff = 0;
    IDSlotAssignment[temporaryIDSlotIndex] = 0;   
}

void Note::executeNoteState(){
  Serial3.write(noteState);//send note on or note off command
  Serial3.write(noteInput);//send pitch data
  Serial3.write(noteVelocity);//send velocity data 
 
}
void Note::augmentNote(){ 
  if(sharpFlat == '#')
    augmentVal = 1;
  else if(sharpFlat == 'b')
    augmentVal = -1;
  //if setoctave param is 0 don't do anything with this block
  if(setOctave == 0);
  else
    applyOctave = setOctave*12;
}
//|| !(noteInput >= 'A' && noteInput <= 'G')

//make a friend function
void Note::validateInput(){ 
  //Serial.println((int)noteInput);
  if(!(noteInput >= 'a' && noteInput <= 'g'))
  {
    Serial.print("Not a valid Note \n");
  }
  else {
    // If you want to check the values that are output
    //remember they will be Mostly Caps because we are setting the midi notes with ascii not the other way around

      switch(noteInput){
       case 'a':
       case 'A':
       
         noteInput = 69+applyOctave + augmentVal;
         break;
         
       case 'b':
       case 'B':
         noteInput = 71+applyOctave+augmentVal;
         break;
         
        case 'c':
        case 'C':
          noteInput = 72+applyOctave+augmentVal;
         break;
         
       case 'd':
       case 'D':
         noteInput = 74+applyOctave+augmentVal;
         break;
         
        case 'e':
        case 'E':
          noteInput = 76+applyOctave+augmentVal;
          break;
          
       case 'f':
       case 'F':
         noteInput = 77+applyOctave+augmentVal;
         break;
         
        case 'g':
        case 'G':
          noteInput = 79+applyOctave+augmentVal;
          break;
          
       default:
         noteInput = 69+applyOctave+augmentVal;
         break;
      }
      savedOctave = noteInput;
  }
}




//Note note1('a', 40); note,velocity

//Note note1('a', 40, 2); note,velocity,octave

//.changeNoteOctave(2)
//Note(char noteInput, char sharpFlat,int MIDIvelocity, int octave) note, # or b , velocity, octave

  //Note note1point5('a','#', 40, 1);
  
  int delayRate = 60;
  Note note1('a', 80, 2); 
  Note note2point5('b','b', 40, 0);
  Note note2('b', 80); 
  Note note3('c', 23);
  Note note4('d', 18);
  Note note5('e', 30);
  Note note6('f', 14);
  Note note7('g', 20);
  
void setup() {
  Serial3.begin(31250);
  Serial.begin(9600);
  Serial3.write(225);
  Serial3.write(63);
  Serial3.write(63);
  
  
 
}

void loop() {

  note4.noteOn();
  note6.noteOn();
  note1.changeNoteOctave(1);
  note1.noteOn();
  note3.changeNoteOctave(1);
  note3.noteOn();
  delay(2000);
  
  
  /*
 Serial3.write(224);
 for(int i = 63; i <127; i++){
   delay(5);
 Serial3.write(i);
 Serial3.write(i);
 }
  for(int i = 127; i >0; i--){
   delay(5);
 
 Serial3.write(i);
 Serial3.write(i);
 }
  Serial3.write(225);
  Serial3.write(63);
  Serial3.write(63);
  */
   note4.noteOff();
   note6.noteOff();
   
   note1.noteOff();
   note3.noteOff();

   note5.noteOn();
   note7.noteOn();
   note2.noteOn();
   delay(2000);
   note5.noteOff();
   note7.noteOff();
   note2.noteOff();
  
   note4.noteOn();
   note6.noteOn();
   note1.changeNoteOctave(-2);
   note1.noteOn();
   delay(2000);
   note4.noteOff();
   note6.noteOff();
   note1.noteOff();
}




















  
/*
  for(int i = -4; i <= 4; i ++){
  note7.changeNoteOctave(i);
  note7.noteOn();
  delay(delayRate);
  note7.noteOff();
  
}

 for(int i = -4; i <= 4; i ++){
  
  note2point5.changeNoteOctave(i);
  note2point5.noteOn();
  delay(delayRate);
  note2point5.noteOff();
  
}

for(int i = -4; i <= 4; i ++){
  
  note4.changeNoteOctave(i);
  note4.noteOn();
  delay(delayRate);
  note4.noteOff();
}

*/
