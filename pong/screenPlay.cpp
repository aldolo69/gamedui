#include <Arduino.h>
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"

#include "audio.h"
#include "pong.h"

#include "math.h"
/*

  playfield 16X16
**** *****    ** -0 <--score row for 2 player
****************

        *      *
       *       *
        *      *
       *       *

*      *
* *     *
*      *
*       *



**************** -15
*/

/*
  start player 1. wait for hit the ball (1)
  (reset ball position)


  pingpong until ball lost (2)
  points + 1
  points=4 and other player <4?-->win game (3)
  points=4 and other player =4?-->advantage
  points=4 and advantage?-->win game (3)
  points=4 and other =advantage?-->the other loose advantage
  if win game
  win=win+1
    win=3-->win match (4)



*/


char cSPinGameplayer = 1; //1..2..0

//1=force redraw next time screenmenu start
char cSPredraw = 1;
//before leaving always set to 1 to reset next time you land here


int iSPspeed = 0;
#define statusWait 0
#define statusPlay 1
#define statusEndOfGame 2
#define statusEndOfMatch 3

char cSPstate = 0; //0=wait for start,1=play,2=wait for fail,3=end of match
char cSPplayer = 1; //1 or 2 for the first move
char cSPgamePlayed = 0;

int iSPdx = 0; //ball slope
int iSPdy = 0;
int iSPx = 0; //ball position
int iSPy = 0;
unsigned long ulSPnextMove = 0; //time of next ball move

int iSPpoints1 = 0; //
int iSPpoints2 = 0; //
int iSPmatch = 0; //0,1,2-->end of match



class clSpPlayer
{
  public:
    clSpPlayer *oOtherPlayer = NULL;
    char cPoints;//15 30 40 45.. 1 2 3 4
    char cGames;//
    char cPadY;
    char cPadY_before;
    char cPadX;
    char cPadDY;
    int iAdcPin;
    char cPlayer;
    int iCampione1 = 0;
    int iCampione2 = 0;
    int iCampione3 = 0;

    clSpPlayer(int playerNumber, int adcpin, int xPosition)
    {
      clearPlayer();
      cPlayer = playerNumber;
      iAdcPin = adcpin;
      cPadX = xPosition;
    }

    void clearPlayer()
    {
      cPoints = 0;
      cGames = 0;
      cPadY = 5;
      cPadDY = 4;

    }


    void autoMove()
    {
      int  y;
      y = cPadY;


      if (iSPy < (y + cPadDY / 2) && y > 2)
      {
        y--;

      }
      if (iSPy >= (y + cPadDY / 2) && y < (15 - cPadDY))
      {
        y++;

      }

      if (cPadY !=  y)
      {
        cPadY_before = cPadY;//remeber for special effects during service
        displayLineDy(cPadX, cPadY, cPadDY, _draw_toggle);
        cPadY = y;
        displayLineDy(cPadX, cPadY, cPadDY, _draw_toggle);
      }

    }

    void movePaddle()
    {
      int iLettura = analogRead(iAdcPin);
      int y;
      y = (iLettura + iCampione1 + iCampione2 + iCampione3 ) / 4;
      iCampione3 = iCampione2;
      iCampione2 = iCampione1;
      iCampione1 = iLettura;
      y = 1023 - y;



      //y from 0 to 2^10-1 (1023) to prevent an excessive swing of the pot
      //only a range will be used: 30% of the total swing: 0-->340 0, 341-->680 ->0..15 , 681-->1024 15
      y = y / 64; //0..15 instead 0..1024
      /*        if(y<340)
              {
                  y=0;
              }
              else if(y>681)
              {
                  y = 15;
              }
              else //341..680-->0..339
              {
                  y=((y-341)*15)/339;
              }
      */
      if (y < 2)
      {
        y = 2;
      }
      if ((y + cPadDY) > 15)
      {
        y = 15 - cPadDY;
      }
      /*
            //restriction: during statusWait the player with the ball cant
            //move freely

            if (cSPstate == statusWait && cPlayer == cSPplayer)
            {
              if (y <= (iSPy - cPadDY))
              {
                y = iSPy - cPadDY + 1;
              }
              if (y > iSPy)
              {
                y = iSPy;
              }
            }
      */


      if (cPadY !=  y)
      {
        cPadY_before = cPadY;//remeber for special effects during service
        displayLineDy(cPadX, cPadY, cPadDY, _draw_toggle);
        cPadY = y;
        displayLineDy(cPadX, cPadY, cPadDY, _draw_toggle);
      }



    }

    //2=bounce against wall or pad
    //0=no collision

    char computeSpeed(int *dx, int *dy, int y)
    {
      /*

          <---
              |
              v


        sqr(dx^2+dy*2)=total velocity. it will remain constant
        if the ball y coordinate is in the middle of the paddle then dx simply change sign
        for a perfect reflection.
        if y is not in the middle then dx is less then perfectly mirrored

        reflection= linear function fro 50% to 100% according to distance from the center

        70%
        100%
        70%


        if y< pad position then no reflection happen
        if y>=pad+dy then no reflection happen

        dy velocity can not be > then dx so minimum dx speed is

        x*x+y*y=x2+x2=v2 x=sqr(v2/2)v/1.41

      ** ^ or v








      ** ^ or v




      */





      if (y < cPadY)
        return (0); //no reflection
      if (y >= cPadY + cPadDY)
        return (0); //no reflection




      double xv = *dx;
      double yv = *dy;
      double speed2 = xv * xv + yv * yv ;
      double speed1 = sqrt( speed2) ;
      double halfpadsize = (double)cPadDY / 2;
      double middle = (double)cPadY + halfpadsize;
      double ydistance;

      //>0 if ball hit under the middle point
      ydistance = (
                    middle - (double)y  + //0 in the middle, 2 at the edge
                    + ((double)random(1000)) / 10000.0  - 0.05 // +- 0.1
                  ) / 10;//0... 0.2

      Serial.write("\n%=");
      Serial.print(ydistance);

      if (ydistance > 0) //hit upper half
      {
        ydistance = abs(ydistance);


        if (yv <= 0) //going UP
        {

          yv = yv -  speed1 *  ydistance; //increase speed

        }
        else//dy>0 going DOWN
        {
          yv = yv - speed1 *  ydistance; //decreasse speed
        }
      }
      else//hit lower half
      {
        ydistance = abs(ydistance);

        if (yv <= 0) //going UP
        {

          yv = yv +  speed1 *  ydistance; //decrease speed

        }
        else//dy>0 going DOWN
        {
          yv = yv + speed1 *  ydistance;//increase speed
        }
      }



      xv = sqrt(speed2 - yv * yv );

      if (*dx > 0)
      {
        xv = -xv;
      }


      Serial.write("\nxv=");
      Serial.print(xv);
      Serial.write("\nyv=");
      Serial.print(yv);



      if (abs(xv) < abs(yv))
      {
        speed1 = speed1 / 1.41;

        if (xv <= 0) xv = -speed1;
        else xv = speed1;
        if (yv <= 0) yv = -speed1;
        else yv = speed1;


      }


      *dx = xv;
      *dy = yv;
      Serial.write("\ndx=");
      Serial.print(*dx);
      Serial.write("\ndy=");
      Serial.print(*dy);

      return (2);
    }



    char winGame()
    {
      cPoints = 0;
      oOtherPlayer->cPoints = 0;
      cGames++;
      if (cGames == 3)
      {
        cSPplayer = cPlayer;
        return statusEndOfMatch;
      }

      return statusEndOfGame;
    }


    //15 30 40 45.. 1 2 3 4
    char winBall()
    {
      if (
        (cPoints == 3 && oOtherPlayer->cPoints < 3) //>2 point
        ||
        (cPoints == 4 && oOtherPlayer->cPoints <= 3) //double advantage
      )

      {
        return winGame();
      }

      if (cPoints == 3 && oOtherPlayer->cPoints == 3)
      {
        cPoints++;//advantage
        return statusWait;//next ball
      }

      if (cPoints == 3 && oOtherPlayer->cPoints == 4)
      {
        oOtherPlayer->cPoints = 3; //cancel other advantage
        cPoints++;//advantage
        return statusWait;//next ball
      }



      cPoints++;
      if (cPoints == 4)
      {
        return winGame();
      }
      return statusWait;//next ball

    }

};


clSpPlayer *oSpPlayer1 = NULL;
clSpPlayer *oSpPlayer2 = NULL;




//is there any left pixel?
char screenPlayCheckEnd()
{
  for (int x = 1; x <= 14; x++)
  {
    for (int y = 1; y <= 7; y++)
    {
      if (!(x == iSPx && y == iSPy))
      {
        if (displayPixelState(x, y) != 0)
          return 0;
      }
    }
  }
  return 1;//no pixel left to be removed
}


//first redraw of the screen
void screenPlayRedraw()
{
  displayClear();
  displayLineDx(0, 1, 16, 0);
  displayLineDx(0, 15, 16, 0);
  for (int y = 2; y < 15; y += 2)
  {
    displayPixel(7, y, _draw_set);
    displayPixel(8, y + 1, _draw_set);
  }

  //draw points and won matches
  if (oSpPlayer1->cPoints != 0)
    displayLineDx(0, 0, oSpPlayer1->cPoints, 0);
  if (oSpPlayer2->cPoints != 0)
    displayLineDx(16 - oSpPlayer2->cPoints, 0, oSpPlayer2->cPoints, 0);
  if (oSpPlayer1->cGames != 0)
    displayLineDx(8 - oSpPlayer1->cGames, 0, oSpPlayer1->cGames, 0);
  if (oSpPlayer2->cGames != 0)
    displayLineDx(8, 0, oSpPlayer2->cGames, 0);


  displayLineDy(0, oSpPlayer1->cPadY, oSpPlayer1->cPadDY, 0);
  displayLineDy(15, oSpPlayer2->cPadY, oSpPlayer2->cPadDY, 0);

  displayPixel(iSPx, iSPy, _draw_set);



  //   displayShowIconXY(_icon_schema_1a+iSPscreen+iSPscreen,0,0,0);
  //   displayShowIconXY(_icon_schema_1b+iSPscreen+iSPscreen,8,0,0);
  //  displayLineDy(0,0,16,0);
  // displayLineDy(15,0,16,0);
  //displayLineDx(0,0,16,0);
  //displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
  // for(int i=1; i<iSPballs; i++)
  // {
  //     displayPixel(2+i+i,15,_draw_set);
  // }

}

//do not redraw the background
/*void screenPlayRedrawBalls()
  {

  displayPixel(iSPx,iSPy,_draw_toggle);

     displayLineDy(0,oSpPlayer1->cPadY,oSpPlayer1->cPadDY,0);
    displayLineDy(15,oSpPlayer2->cPadY,oSpPlayer2->cPadDY,0);


    // int i=1;
    //displayPixel(iSPx,iSPy,_draw_reset);//

    // displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
    // for(; i<iSPballs; i++)
    //{
    //    displayPixel(2+i+i,15,_draw_set);
    // }
    //for(; i<SPballs; i++)
    //{
    //     displayPixel(2+i+i,15,_draw_reset);
    //  }

  }

*/



//return=1-->error! ball lost
//2=bounce against wall or pad
//0=no collision
char screenPlayBounce(int iX, int iY)
{
  char cRet = 0;

  if (iY <= 1) //top wall
  {
    iSPdy = -iSPdy; //reflected
    cRet = 2;
  }
  if (iY > 14) //bottom wall
  {
    iSPdy = -iSPdy; //reflected
    cRet = 2;
  }


  if (iX < 1 && iSPdx < 0)
  {
    cRet = oSpPlayer1->computeSpeed(&iSPdx, &iSPdy, iY);
    if (cRet == 0 && (iX < 0 || iX > 15))
    {
      cRet = 1; //ball lost!!!
    }
  }



  if (iX > 14 && iSPdx > 0)
  {
    cRet = oSpPlayer2->computeSpeed(&iSPdx, &iSPdy, iY);
    if (cRet == 0 && (iX < 0 || iX > 15))
    {
      cRet = 1; //ball lost!!!
    }
  }







  return cRet;
}



/*
  how to use slope: 8 octane to apply Bresenham's line algorithm

  2 1
  3   0
  4   7
  5 6

  i need to move by 1 pixel, according to slope.
  whenever slope change the algorithm start from fresh
  if the slope stay constant then the algorithm goes on keeping the eps error running
*/

void screenPlayMoveStep(int x, int y, int dx, int dy, int *eps, int *iNextX, int *iNextY, int addX, int addY)
{
  *eps += dy;
  (*iNextX) += addX;
  if ( ((*eps) << 1) >= dx )
  {
    (*iNextY) += addY;
    *eps -= dx;
  }
}



//compute next position 1 pixel away according to slope
void screenPlayMove(int * piNextX, int * piNextY)
{
  //slope according to iSPdx/iSPdy. which is the preset octane?
  char cSlope = 0;
  if (iSPdx < 0)
  {
    //2,3,4,5
    if (iSPdy < 0) //iSPdx<0
    {
      //4,5
      if (iSPdx < iSPdy)
      {
        //4
        cSlope = 4;
      }
      else
      {
        //5
        cSlope = 5;
      }
    }
    else//iSpDy>=0 iSPdx<0
    {
      //2,3
      if ((-iSPdx) > iSPdy)
      {
        //3
        cSlope = 3;
      }
      else
      {
        //2
        cSlope = 2;
      }
    }
  }
  else //iSPdx>=0
  {
    //0,1,7,6
    if (iSPdy < 0) //iSPdx>=0
    {
      //6,7
      if ((-iSPdy) > iSPdx) //iSPdx>=0
      {
        //6
        cSlope = 6;
      }
      else
      {
        //7
        cSlope = 7;
      }
    }
    else
    {
      //0,1
      if ( iSPdy > iSPdx) //iSPdx>=0
      {
        //1
        cSlope = 1;
      }
      else
      {
        //0
        cSlope = 0;
      }
    }

  }


  /*
    for octane 0 it is possible to use:
    iSPx,iSPy=initial position
    iSPdx,iSPdy=delta
  */

  static int eps = 0;
  static int oldDx = 9999;
  static int oldDy = 9999;

  if (oldDx != iSPdx || oldDy != iSPdy)
  {
    //initialize algorithm
    oldDx = iSPdx;
    oldDy = iSPdy;
    eps = 0;
  }

  switch (cSlope)
  {
    case 0:
      //        screenPlayMoveStep(x, y);
      screenPlayMoveStep(iSPx, iSPy, iSPdx, iSPdy, &eps, piNextX, piNextY, 1, 1);
      break;
    case 1:
      //        screenPlayMoveStep(y, x);
      screenPlayMoveStep(iSPy, iSPx, iSPdy, iSPdx, &eps, piNextY, piNextX, 1, 1);
      break;
    case 2:
      //        screenPlayMoveStep(y, -x);
      screenPlayMoveStep(iSPy, iSPx, iSPdy, -iSPdx, &eps, piNextY, piNextX, 1, -1);
      break;
    case 3:
      //        screenPlayMoveStep(-x, y);
      screenPlayMoveStep(iSPx, iSPy, -iSPdx, iSPdy, &eps, piNextX, piNextY, -1, 1);
      break;
    case 4:
      //        screenPlayMoveStep(-x, -y);
      screenPlayMoveStep(iSPx, iSPy, -iSPdx, -iSPdy, &eps, piNextX, piNextY, -1, -1);
      break;
    case 5:
      //        screenPlayMoveStep(-y, -x);
      screenPlayMoveStep(iSPy, iSPx, -iSPdy, -iSPdx, &eps, piNextY, piNextX, -1, -1);
      break;
    case 6:
      //        screenPlayMoveStep(-y, x);
      screenPlayMoveStep(iSPy, iSPx, -iSPdy, iSPdx, &eps, piNextY, piNextX, -1, 1);
      break;
    case 7:
      //        screenPlayMoveStep(x, -y);
      screenPlayMoveStep(iSPx, iSPy, iSPdx, -iSPdy, &eps, piNextX, piNextY, 1, -1);
      break;
  }

}




//play task
char screenPlay(void)
{
  unsigned long ulNow = millis();

  if (cSPredraw == 1) //first time here. reset all
  {
    //reset all!!!!

    if (oSpPlayer1 == NULL)
    {
      oSpPlayer1 = new clSpPlayer(1, 1, 0);
      oSpPlayer2 = new clSpPlayer(2, 3, 15);
      oSpPlayer1->oOtherPlayer = oSpPlayer2;
      oSpPlayer2->oOtherPlayer = oSpPlayer1;
    }

    oSpPlayer1->clearPlayer();
    oSpPlayer2->clearPlayer();

    iSPspeed = iSMspeed;


    cSPredraw = 0;

    cSPgamePlayed = 0;
    iSPx = 1; //ball position
    iSPy = 6;
    iSPdx = 100;
    iSPdy = 0;
    cSPstate = statusWait;
    screenPlayRedraw();
    ulSPnextMove = 0;

  }


  //check paddle position. if changed then move them
  if (cSPstate == statusPlay || cSPstate == statusWait) //wait or play
  {
    static unsigned long lNextMove = 0;

    if (cSPinGameplayer >= 1)
    {
      oSpPlayer1->movePaddle();
    }
    else
    {
      if (ulNow > lNextMove && iSPdx < 0)
      {
        oSpPlayer1->autoMove();
      }

    }
    if (cSPinGameplayer >= 2)
    {
      oSpPlayer2->movePaddle();
    }
    else
    {
      if (ulNow > lNextMove && iSPdx > 0)
      {
        oSpPlayer2->autoMove();
      }

    }
    if (ulNow > lNextMove)
    {
      lNextMove = ulNow + 100;
    }

  }



  if (ulSPnextMove < ulNow)
  {
    ulSPnextMove = ulNow + 25 * (9 - iSPspeed) + 50;
    //move the ball
    //iSPx,iSPy position
    // iSPdx,iSPdy delta
    if (cSPstate == statusPlay) //play
    {
      //which is the next ball position? add delta and check for:
      //-wall bounce-->new position is not inside a wall so apply reflection

      int iNextX = iSPx;
      int iNextY = iSPy;
      screenPlayMove(&iNextX, &iNextY);
      switch (screenPlayBounce(iNextX, iNextY))
      {
        case 0://go straight
          displayPixel(iSPx, iSPy, _draw_toggle);
          iSPx = iNextX;
          iSPy = iNextY;
          displayPixel(iSPx, iSPy, _draw_toggle);
          break;
        case 1://error
          if (cSMsound != 0)
          {
            play_rtttl_stop();
            play_rtttl_start_no_repeat(soundLoose);
          }

          if (iSPx < 2)
          { //player 2 win
            cSPstate = oSpPlayer2->winBall();
            cSPplayer = 2;
            if (cSPinGameplayer < 2) oSpPlayer2->cPadY = 10;
            iSPx = 14; //ball position
            iSPy = 11;
            iSPdx = -100;
            iSPdy = 0;
            ulSPnextMove = 0;


          }
          else
          { //player 1 win
            cSPstate = oSpPlayer1->winBall();
            iSPx = 1; //ball position
            cSPplayer = 1;
            if (cSPinGameplayer < 1) oSpPlayer1->cPadY = 4;
            iSPy = 5;
            iSPdx = 100;
            iSPdy = 0;
            ulSPnextMove = 0;


          }

          if (cSPstate == statusEndOfGame)

          {
            cSPgamePlayed++;


            if (cSPgamePlayed & 1) //alternate first ball
            {
              cSPplayer = 2;
              if (cSPinGameplayer < 2) oSpPlayer2->cPadY = 10;
              iSPx = 14; //ball position
              iSPy = 11;
              iSPdx = -100;
              iSPdy = 0;
              ulSPnextMove = 0;

            }
            else
            {
              cSPplayer = 1;
              if (cSPinGameplayer < 1)  oSpPlayer1->cPadY = 4;
              iSPx = 1; //ball position
              iSPy = 5;
              iSPdx = 100;
              iSPdy = 0;
              ulSPnextMove = 0;
            }
          }


          //next status could be the end of match display

          if (cSPstate == statusEndOfMatch)
          {
            return _screenEnd;
          }

          if (cSPstate == statusEndOfGame || cSPstate == statusWait)
          {
            screenPlayRedraw();
            cSPstate = statusWait;
          }



          break;
        case 2://bounce.

          if (cSMsound != 0 && iSMtone == 0)
          {
            play_rtttl_start_no_repeat(soundPing);
          }



          break;
      }
    }
  }


  static unsigned long lWait = 0;
  if (cSPstate == statusWait)
  {
    if (lWait == 0)
    {
      lWait = ulNow + 500;
    }
    if (ulNow < lWait)
      return _screenPlay;
  }
  else
  {
    lWait = 0;
  }


  int iKeyPressed = keyboardGetKey();
  if (cSPinGameplayer < 2 && cSPstate == statusWait)
  {
    if ((cSPplayer == 1 && cSPinGameplayer == 0)
        ||
        (cSPplayer == 2 && cSPinGameplayer <= 1)
       )
    {
      iKeyPressed = enter_key;
    }
  }

  switch (iKeyPressed)
  {
    case up_key:
      cSPredraw = 1; //exit
      return _screenMenu;
      break;
    case enter_key:
      if (cSPstate == statusWait) //play ball!!!!
      {
        
        if (cSPplayer == 1)
        {
          iSPdx = 100;
          iSPdy = 0;
           if( (oSpPlayer1->cPadY <= (iSPy - oSpPlayer1->cPadDY))
            ||  (oSpPlayer1->cPadY > iSPy))
          {
            }
          else
          {
        cSPstate = statusPlay;
        oSpPlayer1->computeSpeed(&iSPdx, &iSPdy, iSPy);
          }
        }
        else
        {
          iSPdx = -100;
          iSPdy = 0;
//can't hit the ball if not behind
           if( (oSpPlayer2->cPadY <= (iSPy - oSpPlayer2->cPadDY))
            ||  (oSpPlayer2->cPadY > iSPy))
          {
            }
          else
          {
        cSPstate = statusPlay;
          oSpPlayer2->computeSpeed(&iSPdx, &iSPdy, iSPy);
        }}





        if (cSMsound != 0)
        {
          play_rtttl_stop();
          play_rtttl_start_no_repeat(soundPing);
        }

      }
      /*   if(cSPstate==0)
         {
             //start play
             cSPstate=1;
         //    iSPdx=(-7+iSPpadX)*10;
             iSPdy=-5*10;
             if(cSMsound!=0)
             {
                 play_rtttl_start(iSMtone);
             }
         }
      */
      break;
    case down_key:
      break;
    case left_key:
      /*       if(iSPpadX>1)
             {
                 //move pad
                 displayLineDx(iSPpadX,14,iSPpadDx,_draw_reset);
                 if(cSPstate==0)
                 {
                     //move ball with pad
                     displayPixel(iSPx,iSPy,_draw_reset);
                     iSPx--;
                 }
                 iSPpadX--;
                 displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
                 if(cSPstate==0)
                 {
                     //move ball with pad
                     displayPixel(iSPx,iSPy,_draw_set);
                 }
             }
      */
      break;
    case right_key:
      /*
              if(iSPpadX+iSPpadDx<15)
              {
                  displayLineDx(iSPpadX,14,iSPpadDx,_draw_reset);
                  if(cSPstate==0)
                  {
                      //move ball with pad
                      displayPixel(iSPx,iSPy,_draw_reset);
                      iSPx++;
                  }
                  iSPpadX++;
                  displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
                  if(cSPstate==0)
                  {
                      //move ball with pad
                      displayPixel(iSPx,iSPy,_draw_set);
                  }
              }
      */
      break;
  }

  return _screenPlay;
}
