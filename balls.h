class balls
{
  public:
    const int rball = 7;
    float xball = 247, yball = 579, value = 0, xendball=247, yendball=579;
    float xdelta, ydelta;
    int increaseball = 0;
    void moveball(int first)
    {
        if (value == 1)
        {
            xball += xdelta;
            yball -= ydelta;
        }

        ballcontrolblock(first);
    }
    void control(int first)
    {
        if (ydelta <0.0005 && ydelta>-0.0005)
            ydelta = 0.01;
        if (xball - rball <= 2)
            xdelta *= -1;
        else if (yball - rball <= 102)
            ydelta *= -1;
        else if (xball + rball >= 492)
            xdelta *= -1;
        else if (yball + rball >= 586)
        {
            first++;
            xendball = xball;
            yendball = yball;
            value = -1;
        }
    }
    void ballcontrolblock(int first)
    {
        control(first);
       
        bool r = true;
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 6; j++)
            {

                if (rect[i][j].value > 0)
                {
                   
                    if (rect[i][j].xrect + 80 < xball && rect[i][j].yrect + 50 < yball && (((xball - rect[i][j].xrect - 80) * (xball - rect[i][j].xrect - 80)) + ((yball - rect[i][j].yrect - 50) * (yball - rect[i][j].yrect - 50))) <= rball * rball)
                    {
                        float q;
                         q = xdelta;
                        xdelta = ydelta;
                        ydelta = q;
                        rect[i][j].touch();
                    }
                    else if ((rect[i][j].xrect > xball) && (rect[i][j].yrect + 50 < yball) && ((rect[i][j].xrect - xball) * (rect[i][j].xrect - xball) + (yball - rect[i][j].yrect - 50) * (yball - rect[i][j].yrect - 50) <= rball * rball))
                    {
                        float q;
                        q = xdelta * -1;
                        xdelta = ydelta * -1;
                        ydelta = q;
                        rect[i][j].touch();
                    }
                    else if (rect[i][j].xrect > xball && yball > rect[i][j].yrect && (rect[i][j].xrect - xball) * (rect[i][j].xrect - xball) + (yball - rect[i][j].yrect) * (yball - rect[i][j].yrect) <= rball)
                    {
                        float q;
                        q = xdelta;
                        xdelta = ydelta;
                        ydelta = q;
                        rect[i][j].touch();
                    }
                    else if (xball > rect[i][j].xrect - 80 && yball < rect[i][j].yrect && (xball - rect[i][j].xrect - 80) * (xball - rect[i][j].xrect - 80) + (rect[i][j].yrect - yball) * (rect[i][j].yrect - yball)<=rball)
                    {
                        float q;
                        q = xdelta * -1;
                        xdelta = ydelta * -1;
                        ydelta = q;
                        rect[i][j].touch();
                    }
                    else if ((xball >= rect[i][j].xrect && xball <= rect[i][j].xrect + 80) && (yball + rball >= rect[i][j].yrect) && (yball < rect[i][j].yrect + 50))
                    {
                        ydelta *= -1;
                        rect[i][j].touch();
                        r = false;
                    }
                    else if ((xball >= rect[i][j].xrect && xball <= rect[i][j].xrect + 80) && (yball - rball <= rect[i][j].yrect + 50) && (yball > rect[i][j].yrect))
                    {
                        ydelta *= -1;
                        rect[i][j].touch();
                        r = false;
                    }
                    else if ((yball >= rect[i][j].yrect && yball <= rect[i][j].yrect + 50) && (xball + rball >= rect[i][j].xrect) && (xball < rect[i][j].xrect + 80))
                    {
                        xdelta *= -1;
                        rect[i][j].touch();
                        r = false;
                    }
                    else if ((yball >= rect[i][j].yrect && yball <= rect[i][j].yrect + 50) && (xball - rball <= rect[i][j].xrect + 80) && (xball > rect[i][j].xrect))
                    {
                        xdelta *= -1;
                        rect[i][j].touch();
                        r = false;
                    }
                }
                else if (rect[i][j].value == -1)
                {
                    if (((xball - rect[i][j].xrect - 40) * (xball - rect[i][j].xrect - 40)) + ((yball - rect[i][j].yrect - 25) * (yball - rect[i][j].yrect - 4)) <= ((12 + rball) * (12 + rball)))
                    {
                        rect[i][j].value = 0;
                        increaseball++;
                        r = false;
                    }
                }
                if (r == false)
                    break;
            }
            if (r == false)
                break;
        }
    }
}gball[100];