/*
20150608
Reweighting 

c  ln[P(E,T)] = ln[n(E)exp(-E/RT)] = ln[n(E)] - E/RT.

input:
   fort.5  #temperature
   fort.20 #Potential energy and ln[n(E)]
output:
   fort.40 #Potential energy and ln[P(E,T)]
c********************************************************************
c  The discontinuous ln[n(E)] --> continuous ln[n(E)].
c
c********************************************************************/
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;
/*	implicit real*8 (a-h,o-z)

        parameter (ndat=100000)
        dimension xx(ndat),yy(ndat)          */
#define NDAT 100000

int main(int argc, char *argv[]){
   vector<double> xx, yy;

   double rgas=8.31451/4.184/1000.0;
/*
c*******************************************************************
c  Set general parameter(s).
        rgas=8.31451d0/4.184d0/1000.0d0
c*******************************************************************
c  Input T.

        read(5,*) temp
        print*,' '
        print*,'  T (K) = ',temp
        print*,' '
c****/
   double temp;
   ifstream ifs_temp("fort.5",ifstream::in);
   if( ifs_temp.fail() ){
      //cerr<<"failed to open file, fort.5 (temperature)"<<endl;
      //cout<<"try to read from standard input (./a.exe < aaa)\n";
      cin >> temp;
      //return 1;
   }
   ifs_temp >> temp;
   ifs_temp.close();   
   //cout<<"\n  T (K) = "<<argv[1]<<endl<<endl;  // argv[1]: temp
   cout<<"\n  T (K) = "<<temp<<endl<<endl;  // argv[1]: temp
   /*c  Input ln[n(E)].

        icou=0

800     continue

        read(20,*,end=900) aaa,bbb
        icou=icou+1

        xx(icou)=aaa
        yy(icou)=bbb

        goto 800   */
   int icou=0; double tmp;
   ifstream ifs20("fort.20",ifstream::in);
   while(ifs20.good()){
   ifs20 >> tmp;
   xx.push_back(tmp);
   ifs20 >> tmp;
   yy.push_back(tmp);
   if(xx[icou]==999999&&yy[icou]==999999)break;
   tmp = 999999;
   icou++;
   //cout << tmp << endl;
   }
   ifs20.close();

   cout<<"N of input data = "<<icou<<endl;
/*c****
900     continue

         print*,'  N of inpit data = ',icou
c****
c  Calculate ln[P(E,T)] = ln[n(E)exp(-E/RT)] = ln(n(E)) - E/RT.

        do ii=1,icou
          yy(ii)= -xx(ii)/rgas/temp + yy(ii) 
        enddo   */
   for(int ii=0;ii<icou;ii++){
      yy[ii]= -1 * xx[ii]/rgas/temp + yy[ii];
   }
/*c****
c  Normalization.

        amax=-100000.0

        do ii=1,icou
          if(yy(ii).gt.amax) amax=yy(ii)
        enddo
        do ii=1,icou
          yy(ii)=yy(ii)-amax
          write(40,133) xx(ii),yy(ii)
        enddo

133     format(f12.3,2x,e20.8)
c*******************************************************************
        stop
        end        */
   double amax = -100000.0;
   for(int ii=0;ii<icou;ii++){
      if(yy[ii] > amax) amax = yy[ii];
   }
   ofstream ofs40;
   ofs40.open("fort.40");
   for(int ii=0;ii<icou;ii++){
      yy[ii] = yy[ii] - amax;
      ofs40<<xx[ii]<<" "<<yy[ii]<<endl;
   }
   ofs40.close();

   return 0;
}
