#ifndef __PROGTEST__
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#define INSIDE 0 // 0000
#define LEFT 1   // 0001
#define RIGHT 2  // 0010
#define BOTTOM 4 // 0100
#define TOP 8    // 1000
#endif /* __PROGTEST__ */
int almostEqual2 ( double x, double y )
{
    if (fabs(x-y)<10e-12*fabs(x+y)) return 1;
    return 0;
}

int computeOutCode(double x, double y,
                   double            rx1,
                   double            ry1,
                   double            rx2,
                   double            ry2){
    int code;
    
    code = 0;          // initialised as being inside of [[clip window]]
    
    if (almostEqual2(x,fmax(rx1,rx2)) || almostEqual2(x,fmin(rx1,rx2))){
        code |= 0;
    } else
        if (x < fmin(rx1,rx2))           // to the left of clip window
            code |= 1;
        else if (fmax(rx1,rx2) < x)      // to the right of clip window
            code |= 2;
    if (almostEqual2(y,fmax(ry1,ry2)) || almostEqual2(y,fmin(ry1,ry2))){
        code |= 0;
    } else
        if (y < fmin(ry1,ry2))           // below the clip window
            code |= 4;
        else if (fmax(ry1,ry2) < y)      // above the clip window
            code |= 8;
    
    return code;
}

int                clipLine                                ( double            rx1,
                                                             double            ry1,
                                                             double            rx2,
                                                             double            ry2,
                                                             double          * ax,
                                                             double          * ay,
                                                             double          * bx,
                                                             double          * by )
{
    double oax,oay,obx,oby; //original values
    *ax *= 1000;
    *ay *= 1000;
    *bx *= 1000;
    *by *= 1000;
    rx1 *= 1000;
    rx2 *= 1000;
    ry1 *= 1000;
    ry2 *= 1000;
    oax = *ax;
    oay = *ay;
    obx = *bx;
    oby = *by;
    int outcodeA = computeOutCode(*ax,*ay,rx1,ry1,rx2,ry2);
    int outcodeB = computeOutCode(*bx,*by,rx1,ry1,rx2,ry2);
    
    
    while (1) {
        
        if (!(outcodeA | outcodeB)) {
            //both points inside the clipping window
            break;
        } else if (outcodeA & outcodeB) {
            //both points share the same zone, cannot be clipped
            if (fabs(*ax-*bx)<=10e-12*fabs(*ax+*bx) && fabs(*ay-*by)<=10e-12*fabs(*ax+*bx)) {
            *ax = oax/1000;
            *ay = oay/1000;
            *bx = obx/1000;
            *by = oby/1000;
            } else {
            *ax /= 1000;
            *ay /= 1000;
            *bx /= 1000;
            *by /= 1000;}
            return 0;
            break;
        } else {
            double x = 0, y = 0;
            //pick point from outside the clipping window
            int outcodeOut = outcodeA ? outcodeA : outcodeB;
            
            if (outcodeOut & 8) {           // point is above the clip window
                x = *ax + (*bx - *ax) * (fmax(ry1,ry2) - *ay) / (*by - *ay);
                y = fmax(ry1,ry2);
            } else if (outcodeOut & 4) { // point is below the clip window
                x = *ax + (*bx - *ax) * (fmin(ry1,ry2) - *ay) / (*by - *ay);
                y = fmin(ry1,ry2);
            } else if (outcodeOut & 2) {  // point is to the right of clip window
                y = *ay + (*by - *ay) * (fmax(rx1,rx2) - *ax) / (*bx - *ax);
                x = fmax(rx1,rx2);
            } else if (outcodeOut & 1) {   // point is to the left of clip window
                y = *ay + (*by - *ay) * (fmin(rx1,rx2) - *ax) / (*bx - *ax);
                x = fmin(rx1,rx2);
            }
            
            if (outcodeOut == outcodeA) {
                *ax = x;
                *ay = y;
                outcodeA = computeOutCode(*ax,*ay,rx1,ry1,rx2,ry2);
            } else {
                *bx = x;
                *by = y;
                outcodeB = computeOutCode(*bx,*by,rx1,ry1,rx2,ry2);
            }
        }
        
    }
    *ax /= 1000;
    *ay /= 1000;
    *bx /= 1000;
    *by /= 1000;
    return 1;
}

#ifndef __PROGTEST__
int almostEqual ( double x, double y )
{
    if (fabs(x-y)<10e-12*fabs(x+y)) return 1;
    return 0;
}

int                main                                    ( void )
{
  double x1, y1, x2, y2;

  x1 = 60;
  y1 = 40;
  x2 = 70;
  y2 = 50;
  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 60 )
           && almostEqual ( y1, 40 )
           && almostEqual ( x2, 70 )
           && almostEqual ( y2, 50 ) );

  x1 = 0;
  y1 = 50;
  x2 = 20;
  y2 = 30;
  assert ( clipLine ( 90, 100, 10, 20, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 10 )
           && almostEqual ( y1, 40 )
           && almostEqual ( x2, 20 )
           && almostEqual ( y2, 30 ) );

  x1 = 0;
  y1 = 30;
  x2 = 120;
  y2 = 150;
  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 10 )
           && almostEqual ( y1, 40 )
           && almostEqual ( x2, 70 )
           && almostEqual ( y2, 100 ) );

  x1 = -10;
  y1 = -10;
  x2 = -20;
  y2 = -20;
  assert ( ! clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 ) );

  x1 = 0;
  y1 = 30;
  x2 = 20;
  y2 = 10;
  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 10 )
           && almostEqual ( y1, 20 )
           && almostEqual ( x2, 10 )
           && almostEqual ( y2, 20 ) );

  x1 = 0;
  y1 = 0.3553;
  x2 = 10.45;
  y2 = 0;
  assert ( clipLine ( 0.95, 0.323, 1, 1, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 0.95 )
           && almostEqual ( y1, 0.323 )
           && almostEqual ( x2, 0.95 )
           && almostEqual ( y2, 0.323 ) );
    

  return 0;
}
#endif /* __PROGTEST__ */
