

void handeln()
{

  /*if(abs(kompass) > 5 && abs(kompass) < 45)
  {
    kompass/kk = signum(kompass)*3;
  }
  else if(abs(kompass) < 45)
  {
    kompass/kk = signum(kompass)*4;
  }
  else
  {
    kompass/kk = signum(kompass)*10;
  }*/

  if (sieht_weiss)
   {

      if(weisswinkel > 0)
      {
        fahre(weisswinkel-180, 30-(weissabstand*2/3),kompass/kk);
      }
      else
      {
        fahre(weisswinkel+180, 30-(weissabstand*2/3),kompass/kk);
      }
      if(abs(weisswinkel) > 170)
      {
        delay(200);
      }
      delay(0);
    }

    
    
  else
  {
    if(near)
    {
      if(signum(ballrichtung) != signum(vtorrichtung))
      {
        zsm = ballrichtung + (vtorrichtung*10/6);
      }
      else
      {
        zsm = (ballrichtung + (vtorrichtung*10/6))/2;
      }
    //fahre(0,20,kompass/kk);
  //Serial.println("near");
    if(ballrichtung == -999)fahre(-180, spee, kompass/kk);
    else if(licht)
    {
      fahre(zsm,75,kompass/kk);
      if(kickzeit > 500)
      {
        kick(15);
        kickzeit = 0;
      }
    }
    else if((abs(ballrichtung)<10))
    {
      if(ballentfernung >= 12)
      {
        fahre(zsm,40,(kompass/kk));
      }
      else
      {
        fahre(ballrichtung,spee,kompass/kk);
      }
    }
    else if(abs(ballrichtung)<15) fahre(ballrichtung,spee,kompass/kk);
    else if(abs(ballrichtung)<20) fahre((signum(ballrichtung)*10),spee*6/7,kompass/kk);
    else if(abs(ballrichtung)<25) fahre((signum(ballrichtung)*30),spee*6/7,kompass/kk);
    else if(abs(ballrichtung)<30) fahre((signum(ballrichtung)*50),spee*6/7,kompass/kk);
    else if(abs(ballrichtung)<35) fahre((signum(ballrichtung)*60),spee*4/5,kompass/kk);
    else if(abs(ballrichtung)<40) fahre((signum(ballrichtung)*80),spee*4/5,kompass/kk);
    else if(abs(ballrichtung)<45) fahre((signum(ballrichtung)*90),spee*4/5,kompass/kk);
    else if(abs(ballrichtung)<50) fahre((signum(ballrichtung)*120),spee,kompass/kk);
    else if(abs(ballrichtung)<60) fahre((signum(ballrichtung)*140),spee,kompass/kk);
    else if(abs(ballrichtung)<90) fahre((signum(ballrichtung)*130),spee,kompass/kk);
    else if(abs(ballrichtung)<100) fahre((signum(ballrichtung)*150),spee,kompass/kk);
    else if(abs(ballrichtung)<120) fahre((-1)*signum(ballrichtung)*180,spee*2/3,kompass/kk);
    else if(abs(ballrichtung)<140) fahre((-1)*signum(ballrichtung)*165,spee,kompass/kk);
    else if(abs(ballrichtung)<165) fahre((-1)*signum(ballrichtung)*150,spee,kompass/kk);
    else if(abs(ballrichtung)>165) fahre((-1)*signum(ballrichtung)*110,spee,kompass/kk);
    //fahre(20,10,0);
    }
    else
    {
      //Serial.println("not near");
      if(!onsht)
      {
        fahre(180,spee/3,kompass/kk);
      }
      else if(htorentfernung < 6)
      {
        fahre(htorrichtung,spee,kompass/kk);
      }
      else if(htorentfernung < 8)
      {
        fahre(htorrichtung,spee/2,kompass/kk);
      }
      else if(htorentfernung < 10)
      {
        fahre(htorrichtung,spee/4,kompass/kk);
      }
      else
      {
        aus();
      }
    }
  }
}





