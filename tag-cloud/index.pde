// -------------------------------------------------------------------
// README:
//    1. Tested under processing-2.0b7 and processing.js-2.4.1
//    2. Install python2 and execute httpd.py to run a simple server
//       and then open a HTML5 browser with 127.0.0.1:9292
//    3. Or run in processing...
// 
// References:
// [1]: Wordle, Jonathan Feinberg
// [2]: PyTagCloud, https://github.com/atizo/PyTagCloud
// [3]: DS and AA in C, Mark Allen Weiss
//
// Email:
//    justmao945 <at> gmail.com
// -------------------------------------------------------------------

// Iterator, Map ...
import java.util.*;

// -------------------------------------------
// Global data declarations
// -------------------------------------------
String[] studioArray; // Lead Studio
String[] storyArray;  // Story
String[] genreArray;  // Genre

// -----------------------------------------------
// Sort helper class(port to processing.js)
// processing has interface?
// -----------------------------------------------
class MyComparable{

  public MyComparable(){}

  // compare function
  public int compareTo(MyComparable other){
    println("unimplemented");
    return 0;
  }

  public int compareString(String a, String b){
    for(int i=0; i<a.length() && i<b.length(); ++i){
      // F**K... can't minus char ?
      // if(a.charAt(i) != b.charAt(i))
        // return a.charAt(i) - b.charAt(i);
      if( a.charAt(i) < b.charAt(i) )
        return -1;
      else if( a.charAt(i) > b.charAt(i) )
        return 1;
    }
    return a.length() - b.length();
  }

  // quick sort
  public void qsort(MyComparable[] data){
    qsortInter(data, 0, data.length - 1);
  }

  // internal sort helper
  private void swap(MyComparable[] data, int a, int b){
    MyComparable c = data[a];
    data[a] = data[b];
    data[b] = c;
  }

  // internal sort helper
  private MyComparable median3(MyComparable[] data, int left, int right){
    int center = int( (left + right) / 2 );
    if( data[left].compareTo(data[center]) > 0 )
      swap(data, left, center);
    if( data[left].compareTo(data[right]) > 0 )
      swap(data, left, right);
    if( data[center].compareTo(data[right]) > 0 )
      swap(data, center, right);

    swap(data, center, right-1); // hide pivot
    return data[right-1]; // return pivot
  }

  // internal sort helper
  private void insertionSort(MyComparable[] data, int left, int right){
    int j, p;
    MyComparable tmp;
    for(p=left+1; p<=right; p++){
      tmp = data[p];
      for(j=p; j>left && data[j-1].compareTo(tmp) > 0; j--)
        data[j] = data[j-1];
      data[j] = tmp;
    }
  }

  // internal sort helper
  private void qsortInter(MyComparable[] data, int left, int right){
    int i, j;
    MyComparable pivot;
    if( left + 3 <= right){
      pivot = median3(data, left, right);
      i = left;
      j = right - 1;
      while(true){
        while( data[++i].compareTo(pivot) < 0 ){}
        while( data[--j].compareTo(pivot) > 0 ){}
        if( i < j )
          swap(data, i, j);
        else
          break;
      }
      swap(data, i, right-1); // restore pivot
      qsortInter(data, left, i-1);
      qsortInter(data, i+1, right);
    }else{
      insertionSort(data, left, right);
    }
  }
}

// -------------------------------------------
// tag cloud configuration class
// ------------------------------------------
class TagConfig{
  public int maxWidth = 500;
  public int maxHeight = 500;
  public color bgColor = color(255, 255, 255, 255);
  public int textMaxSize = 48;
  public int textMinSize = 10;
  public boolean textRandColor = true;
  public color textColor = color(0, 0, 0, 255);
  public boolean textOpacity = false;
  public int textMaxOpacity = 255;
  public int textMinOpacity = 100;
  public int textPadding = 5;
  public String textFont = "Arial";
  public int spiralAngleStep = 10; // step in 5 degrees, max is 360(aka TWO_PI)
  public int spiralRadiusStep = 5; // step in 4 pixel

  // return random color if textRandColor is true
  public int getRandColor(){
    return int(random(256));
  }

  public int getMaxColor(){
    return 255;
  }
}

// -------------------------------------------------
// Tag pairs prepared to sort
// -------------------------------------------------
class TagPairByName extends MyComparable{
  public String name;
  public int weight;

  public TagPairByName(){}

  public TagPairByName(String name, int weight){
    this.name = name;
    this.weight = weight;
  }

  public int compareTo(MyComparable other){
    return compareString(name, ((TagPairByName)other).name);
  }
}

class TagPairByWeight extends MyComparable{
  public String name;
  public int weight;

  public TagPairByWeight(){}

  public TagPairByWeight(String name, int weight){
    this.name = name;
    this.weight = weight;
  }

  public int compareTo(MyComparable other){
    int w = ((TagPairByWeight)other).weight;
    if( w != weight)
      return w - weight;
    else 
      return compareString(name, ((TagPairByWeight)other).name);
  }
}


// -------------------------------------------------
// tag cloud main class
// -------------------------------------------------
class TagCloud{
  TagConfig cfg; // config
  HashMap names; // key is name, value is weight of name

  public TagCloud(){
    names = new HashMap();
    cfg = new TagConfig();
  }

  public TagConfig getConfig(){
    return cfg;
  }

  public void setConfig(TagConfig cfg){
    this.cfg = cfg;
  }
  
  // take a string array to make a tag
  public void parseArray(String[] nameArray){
    for(String name: nameArray){
      if(name.trim().length() == 0)
        continue;
      if( ! names.containsKey(name) )
        names.put(name, 1);
      else
        names.put(name, (Integer)names.get(name) + 1);
    }
  }

  // get name weight, return 0 if not find
  public int getWeight(String name){
    if( !names.containsKey(name) )
      return 0;
    else
      return (Integer)names.get(name);
  }

  // get a sorted array of names
  public TagPairByName[] getSortedNames(){
    TagPairByName[] namesArray = new TagPairByName[names.size()];
    Iterator i = names.entrySet().iterator();
    int index = 0;
    while (i.hasNext()) {
      Map.Entry me = (Map.Entry)i.next();
      namesArray[index] = new TagPairByName((String)me.getKey(), (Integer)me.getValue());
      ++index;
    }
    new TagPairByName().qsort(namesArray);
    // for(TagPairByName n:namesArray)
      // println("> " + n.name + ":" + n.weight);
    return namesArray;
  }

  // get a sorted array of weight
  public TagPairByWeight[] getSortedWeight(){
    TagPairByWeight[] weightArray = new TagPairByWeight[names.size()];
    Iterator i = names.entrySet().iterator();
    int index = 0;
    while (i.hasNext()) {
      Map.Entry me = (Map.Entry)i.next();
      weightArray[index] = new TagPairByWeight((String)me.getKey(), (Integer)me.getValue());
      ++index;
    }
    new TagPairByWeight().qsort(weightArray);
    // for(TagPairByWeight n:weightArray)
      // println("< " + n.name + ":" + n.weight);
    return weightArray;
  }


  // print hash map
  public void printNames(){
    Iterator i = names.entrySet().iterator();
    while (i.hasNext()) {
      Map.Entry me = (Map.Entry)i.next();
      println(me.getKey() + ": " + me.getValue());
    }
  }

  // Draw a simple tag cloud
  public void drawSimple(){
    if( names.size() == 0 ){
      println("Nonthing to draw...");
      return;
    }
    TagPairByName[]   sortedNames = this.getSortedNames();
    TagPairByWeight[] sortedWeight = this.getSortedWeight();
    int minWeight = sortedWeight[sortedWeight.length - 1].weight;
    int maxWeight = sortedWeight[0].weight;
    int x = 0, y = 0, wordSize, wordWidth, lineSpace = 0;
    float scale = 1.0 * (cfg.textMaxSize - cfg.textMinSize) / (maxWeight - minWeight);
    float opacityScale = 1.0 * (cfg.textMaxOpacity - cfg.textMinOpacity) / cfg.getMaxColor();
    PFont font = createFont(cfg.textFont, cfg.textMaxSize, true);
    background(cfg.bgColor);
    for(int i=0; i<sortedNames.length; ++i){
      wordSize = int((sortedNames[i].weight - minWeight) * scale  + cfg.textMinSize);
      textFont(font, wordSize);
      wordWidth = int(textWidth(sortedNames[i].name));
      if(wordSize > lineSpace)
        lineSpace = wordSize;
      if( x + wordWidth > cfg.maxWidth ){ // new line
        if( x == 0 ){ // not enough width, and can't place anywhere.
          println("not enough width, please set a higher maxWidth");
          break;
        }
        x = 0;
        y += lineSpace + cfg.textPadding;
        lineSpace = 0;
      }
      if(cfg.textRandColor){
        if(cfg.textOpacity)
          fill(cfg.getRandColor(), cfg.getRandColor(), cfg.getRandColor(),
               cfg.getRandColor() * opacityScale + cfg.textMinOpacity );
        else
          fill(cfg.getRandColor(), cfg.getRandColor(), cfg.getRandColor());
      }else{
        fill(cfg.textColor);
      }
      text(sortedNames[i].name, x, y + wordSize);
      x += wordWidth + cfg.textPadding;
    }
  }

  // left-bottom: (x,y); right-top: (x1,y1);
  // only detect outline of rect, and this require sorted weight in ascend order.
  public boolean isBlankRect(int x, int y, int x1, int y1, int eps, color bg){
    for(int i=y1; i<=y; i+=eps){ //left and right
      if( get(x, i) != bg || get(x1, i) != bg || get(int((x + x1) / 2), i) !=bg )
        return false;
    }
    for(int i=x; i<=x1; i+=eps){ // top and bottom
      if( get(i, y) != bg || get(i, y1) != bg || get(i, int((y + y1) / 2)) != bg )
        return false;
    }
    return true;
  }

  // stupid detection
  // TODO: optimization for the two loops...
  public boolean __isBlankRect(int x, int y, int x1, int y1, int eps, color bg){
    for(int i=x; i<=x1; i+=eps)
      for(int j=y1; j<=y; j+=eps)
        if( get(i, j) != bg )
          return false;
    return true;
  }

  // Draw a tag cloud through the spiral algorithm
  public void drawSpiral(){
    if( names.size() == 0 ){
      println("Nothing to draw...");
      return;
    }

    TagPairByWeight[] sortedWeight = this.getSortedWeight();
    int minWeight = sortedWeight[sortedWeight.length - 1].weight;
    int maxWeight = sortedWeight[0].weight;
    float scale = 1.0 * (cfg.textMaxSize - cfg.textMinSize) / (maxWeight - minWeight);
    float opacityScale = 1.0 * (cfg.textMaxOpacity - cfg.textMinOpacity) / cfg.getMaxColor();
    int wordSize, wordWidth, x=0, y=0, x1, y1; // left-bottom: (x,y); right-top: (x1,y1);
    PFont font = createFont(cfg.textFont, cfg.textMaxSize, true);
    background(cfg.bgColor);

    for (TagPairByWeight tp: sortedWeight) {
      wordSize = int((tp.weight - minWeight) * scale  + cfg.textMinSize);
      textFont(font, wordSize);
      wordWidth = int(textWidth(tp.name));
      if(cfg.textRandColor){
        if(cfg.textOpacity)
          fill(cfg.getRandColor(), cfg.getRandColor(), cfg.getRandColor(),
               cfg.getRandColor() * opacityScale + cfg.textMinOpacity );
        else
          fill(cfg.getRandColor(), cfg.getRandColor(), cfg.getRandColor());
      }else{
        fill(cfg.textColor);
      }

      // find blank rect
      for(int r=0; 2*r < cfg.maxWidth && 2*r < cfg.maxHeight; r+=cfg.spiralRadiusStep){
        for(int a=0; a<360; a+=cfg.spiralAngleStep) {
          int rx = int(cfg.maxWidth / 2 + r * cos(a / 360.0 * TWO_PI));
          int ry = int(cfg.maxHeight / 2 + r * sin(a / 360.0 * TWO_PI));
          x = rx - int(wordWidth / 2);
          y = ry + int(wordSize / 2);
          x1 = x + wordWidth;
          y1 = y - wordSize;
          if( this.isBlankRect(x, y, x1, y1, 1, cfg.bgColor) ){
            r = cfg.maxWidth; // break outer while loop
            break;
          }
          if(r == 0) a = 360;
        }
      }
      text(tp.name, x, y);
    }
  }
}

// ------------------------------------------------
// Unit test
// ------------------------------------------------
class TestCmp extends MyComparable{
  public int x;
  
  public TestCmp(int x){
    this.x = x;
  }

  public int compareTo(MyComparable other){
    return this.x - ((TestCmp)other).x;
  }
}

void testTagCloud(){
  TagCloud tc = new TagCloud();
  tc.parseArray(studioArray);
  tc.printNames();
  tc.getSortedNames();
  tc.getSortedWeight();

  println(tc.getWeight("Sony"));
  println(tc.getWeight("Sonyxxx"));
  // tc.drawSimple();

  int[] arr = {2, 6, 38, 0, -1, 1000, 128, 99, 0, 10, 23};
  TestCmp[] tcmp = new TestCmp[arr.length];
  for(int i=0; i<arr.length; ++i)
    tcmp[i] = new TestCmp(arr[i]);

  new MyComparable().qsort(tcmp);

  for(TestCmp t: tcmp)
    println(t.x);

  String str = "ABCDEFG";
  char c1 = str.charAt(0);
  char c2 = str.charAt(1);
  char c3 = str.charAt(str.length()-1);
  
  println(c1 + ":" + c2 + ":" + c3 + ":" + (c1 - c2));
  // c1 - c1 NaN ???
}


// -------------------------------
// Setup for processing
// -------------------------------
void setup(){
  size(500, 500);
  smooth();
  studioArray = new String[] { 
    "Independent", "Independent", "Independent", "Lionsgate", "Vertigo Entertainment",
    "Independent", "Relativity Media", "Independent", "Weinstein Company", "Warner Bros",
    "Aardman Animations", "", "Independent", "Relativity Media", "CBS Films", "Independent",
    "Relativity Media", "Disney", "Pixar", "20th Century Fox", "Sony", "Lionsgate", 
    "Warner Bros", "Universal", "Warner Bros", "20th Century Fox", "Independent", "Miramax Films",
    "Morgan Creek Productions", "Independent", "Independent", "Independent",
    "Reliance Entertainment", "Warner Bros.", "Universal", "New Line Cinema", 
    "Spyglass Entertainment", "Warner Bros", "DreamWorks", "Disney", "Warner Bros",
    "Warner Bros", "Universal", "Village Roadshow Pictures", "Warner Bros", "Independent",
    "Relativity Media", "Warner Bros", "Paramount", "Paramount", "Relativity Media", 
    "Regency Enterprises", "Sony", "Warner Bros", "Happy Madison", "Universal", "Relativity Media",
    "Happy Madison", "Independent", "DreamWorks Animation", "Universal", "Virgin", "Lionsgate",
    "Independent", "Disney", "Sony", "Paramount", "Columbia", "20th Century Fox", "20th Century Fox",
    "The Weinstein Company", "Sony", "Warner Bros.", "Spyglass Entertainment", "Independent", 
    "The Weinstein Company", "Independent", "Relativity", "Disney", "Sony", "DreamWorks", 
    "Paramount", "DreamWorks Pictures", "Warner Bros", "Independent", "20th Century Fox", 
    "20th Century Fox", "Relativity Media", "Weinstein Company", "Relativity ", "Independent",
    "Relativity", "Warner Bros", "Independent", "Sony", "Summit Entertainment", "Weinstein Company", 
    "Legendary Pictures", "Paramount", "Relativity", "Independent", "Independent", "Paramount",
    "Weinstein Company", "Summit Entertainment", "Independent", "Relativity Media", 
    "Weinstein Company", "Independent", "Universal", "20th Century Fox", "Spyglass Entertainment",
    "Universal", "Sony", "Columbia", "Legendary Pictures", "DreamWorks Pictures", "Columbia",
    "Weinstein Company", "Lionsgate", "Independent", "Disney", "Warner Bros", "Independent",
    "Independent", "Fox", "Sony Pictures Animation", "Independent", "Universal", "Independent",
    "Independent", "Disney", "Focus", "Relativity", "DreamWorks Pictures", "Independent", "Independent",
    "Independent", "Independent", "Independent", "Disney", "Lionsgate", "20th Century Fox", "Fox", 
    "20th Century Fox", "Disney", "Disney", "Universal", "Happy Madison Productions"
  };

  storyArray = new String[] {
     "Comedy", "Discovery", "Love", "Comedy", "Maturation", "", "Tragedy", 
    "Temptation", "Monster Force", "Sacrifice", "Journey and Return", "",
    "Comedy", "Monster Force", "Metamorphosis", "Love", "Rivalry", "Metamorphosis",
    "Fish Out Of Water", "Maturation", "Revenge", "Quest", "The Riddle", "Monster Force",
    "Discovery", "Comedy", "Quest", "Monster Force", "The Riddle", "Rivalry", "Rescue",
    "Monster Force", "Discovery", "", "Escape", "Monster Force", "Transformation",
    "Temptation", "Rivalry", "Love", "Metamorphosis", "Temptation", "Quest", 
    "Journey and Return", "Rivalry", "Wretched Excess", "Fish Out Of Water", "Revenge",
    "", "Maturation", "Revenge", "The Riddle", "Monster Force", "Wretched Excess", "Comedy",
    "Love", "Comedy", "Comedy", "Rivalry", "Rivalry", "Discovery", "Wretched Excess", 
    "Transformation", "Sacrifice", "Rescue", "Love", "Pursuit", "Underdog", "Love", 
    "Discovery", "Love", "Rivalry", "Love", "Comedy", "Love", "Comedy", "Monster Force",
    "Comedy", "Quest", "Monster Force", "Quest", "Fish Out Of Water", "Underdog", 
    "Monster Force", "Escape", "Quest", "Revenge", "Quest", "Monster Force", 
    "Monster Force", "", "Escape", "Pursuit", "Love", "Underdog", "The Riddle",
    "Quest", "Quest", "Monster Force", "Discovery", "Sacrifice", "Sacrifice", 
    "Quest", "", "Transformation", "Transformation", "Metamorphosis", "Discovery",
    "Underdog", "Pursuit", "Discovery", "Comedy", "Quest", "", "Rivalry", "Comedy",
    "Maturation", "Transformation", "", "Fish Out Of Water", "Revenge", "Quest", 
    "Monster Force", "Monster Force", "Discovery", "", "Fish Out Of Water", "Discovery",
    "Monster Force", "Rivalry", "Discovery", "Monster Force", "", "Revenge", "Quest",
    "Escape", "Love", "The Riddle", "Monster Force", "Love", "", "Rivalry", "Love", 
    "", "Transformation", "Quest", "Monster Force", "Comedy", "Comedy" 
  };

  genreArray = new String[] { 
    "Comedy", "Comedy", "Drama", "Comedy", "Action", "Drama", "Drama", "Fantasy", 
    "Horror", "Comedy", "Animation", "", "Comedy", "Action", "Romance", "Comedy", 
    "Comedy", "Action", "Animation", "Comedy", "Action", "Action", "Thriller", "Action",
    "Romance", "Comedy", "Drama", "Horror", "Horror", "Thriller", "Action", "Horror",
    "Drama", "Drama", "Action", "Horror", "Drama", "Romance", "Horror", "Animation",
    "Action", "Comedy", "Action", "Animation", "Fantasy", "Drama", "Animation", "Comedy",
    "Adventure", "Thriller", "Action", "Thriller", "Horror", "Drama", "Comedy", "Romance",
    "Comedy", "Romance", "Action", "Animation", "Drama", "Thriller", "Action", "Thriller",
    "Animation", "Romance", "Action", "Drama", "Romance", "Comedy", "Drama", "Action",
    "Romance", "Comedy", "Romance", "Comedy", "Horror", "Comedy", "Action", "Action", 
    "Animation", "Animation", "Action", "Horror", "Horror", "Animation", "Action", "Action",
    "Horror", "Action", "Drama", "Horror", "Action", "Romance", "Drama", "Thriller",
    "Action", "Action", "Horror", "Comedy", "Drama", "Thriller", "Adventure", "Drama",
    "Drama", "Comedy", "Comedy", "Drama", "Drama", "Thriller", "Drama", "Comedy", "Action",
    "Thriller", "Action", "Comedy", "Drama", "Thriller", "Drama", "Drama", "Action",
    "Comedy", "Horror", "Horror", "Drama", "Comedy", "Animation", "Thriller", "Horror", 
    "Action", "Drama", "Action", "Thriller", "Comedy", "Action", "Thriller", "Romance",
    "Thriller", "Horror", "Romance", "Drama", "Action", "Drama", "Comedy", "Comedy",
    "Animation", "Action", "Comedy", "Comedy" 
  };
  
  TagCloud tc = new TagCloud();
  tc.parseArray(studioArray);
  // testTagCloud();
  // tc.drawSimple();
  tc.drawSpiral();
}

// -------------------------------
// Drawing
// -------------------------------
void draw(){
}

void mouseMoved(){
}

// vim:set ft=java:
