
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int red, green, blue, yellow;
unsigned long foreground, background;


//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;
	

	int dx = 160;
	int dy = 150;
	XPoint points[11];
	points[0].x = 0+dx;
	points[0].y = 0+dy;
	points[1].x = 120+dx;
	points[1].y = 0+dy;
	points[2].x = 120+dx;
	points[2].y = 190+dy;
	points[3].x = 100+dx;
	points[3].y = 210+dy;
	points[4].x = 10+dx;
	points[4].y = 210+dy;
	points[5].x = 30+dx;
	points[5].y = 190+dy;
	points[6].x = 40+dx;
	points[6].y = 190+dy;
	points[7].x = 80+dx;
	points[7].y = 190+dy;
	points[8].x = 100+dx;
	points[8].y = 190+dy;
	points[9].x = 100+dx;
	points[9].y = 20+dy;
	points[10].x = 0+dx;
	points[10].y = 20+dy;
	

	int dx2 = 640;
	XPoint points2[11];
	points2[0].x = 0+dx2;
	points2[0].y = 0+dy;
	points2[1].x = 120+dx2;
	points2[1].y = 0+dy;
	points2[2].x = 120+dx2;
	points2[2].y = 20+dy;
	points2[3].x = 20+dx2;
	points2[3].y = 20+dy;
	points2[4].x = 20+dx2;
	points2[4].y = 190+dy;
	points2[5].x = 100+dx2;
	points2[5].y = 190+dy;
	points2[6].x = 100+dx2;
	points2[6].y = 170+dy;
	points2[7].x = 80+dx2;
	points2[7].y = 170+dy;
	points2[8].x = 80+dx2;
	points2[8].y = 150+dy;
	points2[9].x = 120+dx2;
	points2[9].y = 150+dy;
	points2[10].x = 120+dx2;
	points2[10].y = 210+dy;
	points2[11].x = dx2;
	points2[11].y = 210+dy;

	
	
	

  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  //okresl rozmiar i polozenie okna
  info.x = 100;
  info.y = 150;
  info.width = 1200;
  info.height = 600;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
      
  to_end = FALSE;

 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput
	XSetForeground(display, gc, green);	
	XFillArc(display, window, gc, 50, 50, 400, 400, 0, 360*64);
	XSetForeground(display, gc, blue);
	XFillArc(display, window, gc, 510, 50, 400, 400, 0, 360*64);
	XSetForeground(display, gc, red);
	XFillPolygon(display, window, gc, points, 11, Complex, CoordModeOrigin);
	XFillPolygon(display, window, gc, points2, 12, Complex, CoordModeOrigin);
	XSetForeground(display, gc, foreground);
	//XDrawLines(display, window, gc, points, 11, 2);
		

    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {
         
        }
        break;

      case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping); // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1)  // sprawdzenie czy wcisnieto lewy przycisk		
        {
   		              
    
        }
        break;


      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }

      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
