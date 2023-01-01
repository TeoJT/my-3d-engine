#include "terminal-display.c"
#define SWAP(x,y) 		do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)



void line(int sx, int sy, int ex, int ey, int c) {
    int dx = ex - sx;
    int dy = ey - sy;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = sx;
    float y = sy;
    for (int i = 0; i < steps; i++) {
        pixel((int)x, (int)y, c);
        x += xIncrement;
        y += yIncrement;
    }
}

void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int color) {
    line((int)x1, (int)y1, (int)x2, (int)y2, color);
    line((int)x2, (int)y2, (int)x3, (int)y3, color);
    line((int)x3, (int)y3, (int)x1, (int)y1, color);
}


/* Horizontal Line */
void lcd_hline(int x1, int x2, int y, int c) 
{
	if (x1 >= x2) 
        SWAP(x1, x2);

	for (; x1 <= x2 ; x1++) 
        pixel(x1, y, c); // replace this with any function in your library of choice
        					// for drawing 1 pixel
}

/* Fill Triangle Function */
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int c) 
{
	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	int changed1 = 0;
	int changed2 = 0;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;

    // Sort vertices
	if (y1 > y2) { SWAP(y1,y2); SWAP(x1,x2); }
	if (y1 > y3) { SWAP(y1,y3); SWAP(x1,x3); }
	if (y2 > y3) { SWAP(y2,y3); SWAP(x2,x3); }

	t1x = t2x = x1; 
    y = y1;   // Starting points

	dx1 = (x2 - x1); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (y2 - y1);
 
	dx2 = (x3 - x1); if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
	dy2 = (y3 - y1);
	
	if (dy1 > dx1) {   // swap values
        SWAP(dx1,dy1);
		changed1 = 1;
	}
	if (dy2 > dx2) {   // swap values
        SWAP(dy2,dx2);
		changed2 = 1;
	}
	
	e2 = (dx2>>1);
    // Flat top, just process the second half
    if(y1==y2) goto next;
    e1 = (dx1>>1);
	
	for (int i = 0; i < dx1;) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
        // process first line until y value is about to change
		while(i<dx1) {
			i++;			
			e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   if (changed1) t1xp=signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
	// Move line
	next1:
        // process second line until y value is about to change
		while (1) {
			e2 += dy2;		
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp=signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	lcd_hline(minx, maxx, y, c);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y==y2) break;
		
   }
	next:
	// Second half
	dx1 = (x3 - x2); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (y3 - y2);
	t1x=x2;
 
	if (dy1 > dx1) {   // swap values
        SWAP(dy1,dx1);
		changed1 = 1;
	} else changed1 = 0;
	
	e1 = (dx1>>1);
	
	for (int i = 0; i<=dx1; i++) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
	    // process first line until y value is about to change
		while(i<dx1) {
    		e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if(i<dx1) i++;
		}
	next3:
        // process second line until y value is about to change
		while (t2x!=x3) {
			e2 += dy2;
	   	   	while (e2 >= dx2) {
				e2 -= dx2;
				if(changed2) t2xp=signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}	   	   
	next4:

		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	lcd_hline(minx, maxx, y, c);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>y3) return;
	}
}
