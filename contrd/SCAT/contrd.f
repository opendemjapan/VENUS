C *** FILE CONTR -----------------
C
C  CONTER3D
C     2002.8.30   M.Mizuno 
C     2002.9.24   C12 removed (PTVAL included in POINGT)  
C     2003.8.21   bug fixed (if CHAR<0, set CHAR to 0)  
C
C  (CONTR coded by ADACH   1995.6.27)
C  2000/11/18  #NZZ expanded (5 to 6)
C              Allighment of KPL1() in PTVAL corrected
C              CN(#IZ *#IZ  )->CN(#DZZ   )
C
C   THIS FILE CONTAINS SUBROUTINES CONTR,POINGT,PTVAL,VALG,PTWAV
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /IOCNTR/IFL1,IFL2,IFL3,IFL4,IFL5,IFL6,IFL7,IFL8,
     & ITSTR,JFL1,JFL2,JFL3,JFL4,JFL5,JFL6,JFL7,JFL8,
     & INPUT,IOUT,IPUN,IEIG,ITP,IYL,ISAV,IEXPC,JPRT,JPUN
      OPEN(04,FILE='c04d')
      OPEN(06,FILE='c06')
      OPEN(09,FILE='f09',FORM='UNFORMATTED')
      OPEN(39,FILE='f39',FORM='UNFORMATTED')
      OPEN(07,FILE='CHG3D.TXT')
      OPEN(08,FILE='POT3D.TXT')
      CALL SFSTOR(0)
      CALL CONTR
      STOP
      END
C   ----------------------------------------------------
C   SUBROUTINE CONTR FOR CONTOUR MAP
C   READ(ICRD=4) DATA FOR MESH POINT
C   CALL-POINGT,PTVAL
C   FILE-ICRD(4),JFL6
C
      SUBROUTINE CONTR
       IMPLICIT REAL*8(A-H,O-Z)
      COMMON / IOCNTR/IFL1,IFL2,IFL3,IFL4,IFL5,IFL6,IFL7,IFL8,
     & ITSTR,JFL1,JFL2,JFL3,JFL4,JFL5,JFL6,JFL7,JFL8,
     & INPUT,IOUT,IPUN,IEIG,ITP,IYL,ISAV,IEXPC,JPRT,JPUN
N     COMMON / PRCORE/ XZX(#C ),XE(#C ,#X),ELC1(#C ,#X),
S     COMMON / PRCORE/XZX(#C ,2),XE(#C ,#X,2),ELC1(#C ,#X,2),
     & JOUT(#C ,#X),NNLZ(#C ),LOCOR(#C ),INDX(#C ),KCORE(#C )
      COMMON / CATCNT/PHM,TOLE,DEL,TOL1,REMV1,REMV2,REMV3,
     & LITERN,ICYCLE,IWRE,NDWAV,NDVR,NMAX,IPOP,NSCF,NSCC,
     & JLV1,JLV2,JLV3,LSY1,LSY2,LSY3,KSP1,KSP2,KSP3,MSTP
      COMMON / PROND/ ZN1(#X),DHD(#X),RS(#X),XALFA(#X),
     & ANUC(#X),RNUC(#X),RW(#X),VW(#X),JCORE(#X),KMAX(#X),
     & NIATOM(#X),NCO(#X),MSTAR(#X),KPLV(#X)
      COMMON / PROCNT/ EXALF,XFAC,TL,RIDO,VIDO,TOLM,
     & KKK2,NCYCLE,NTABPT, NMAXK,NAT,NDAT,NCORE,NATOMS,
     & NTYPES,NSYM,NAOS,NMOS,IIDO,LTIME,NNST,ITAPE,NTAPE,
     & MAOS,MISOS,IDOA,ITIME,NGRD,MSCF,MQ,MMOL(10),
     & ISYML(#M),JSYML(#M),NORB(#M),LORB(#M),MTITL(4,#M)
      COMMON / PROATM/ XVP(#HZ  ),YVP(#HZ  ),ZVP(#HZ  ),Z(#G ),
     & R0(#HZ  ),LMAXZ(#G ),NEQP(#HZ  ),NEQ(#G )
      COMMON / PRAMO/ EIGN1(#EZZ  ),LVAL(#AZ ),LOCA(#AZ ),NBPT(#AZ ),
     & KAT(#AZ ),MCOPT(#AZ ),MVAL(#AZ ),NPART(#EZZ  )
N     COMMON / PRMESH/ RHS(#LZ,#X),RHO(#LZ,#X),NAME(10,#X)
S     COMMON / PRMESH/ RHS(#LZ,#X),RHO(#LZ,#X,2),RHT(#LZ,#X),
S    & NAME(10,#X)
      COMMON / MATOUT/ MVAL1(#AZ ),ICHI(#AZ ),KIDO(#AZ ),
     & ICHIS(20),ICHIT(20),NCHI(#AZ ),MCOR(#AZ )
      COMMON / NATC/ IATC(#X),IATD
      COMMON/PDIST/VECBAS(3),W1TAB(#G ),TICK(#G ),ALF(#G ),BETA(#G ),
     & GAMMA(#G )
      COMMON /CNST/ VCL,PI,EUNIT,ERYD
      COMMON /RELC/YX(8,1)
      COMMON/CONT/ KPRT,IFW,ILW,NPW,MINJ,MAXJ,NWV(100),NWS(100),IPRT
      COMMON/CONT3D/XBL,YBL,ZBL
      COMMON / / XN(#C ),XL(#C ),DXZ(#C ),XZ(#C ,2),A(#C ),B(#C ),
     & XII(3),XV(#HZ  ),YV(#HZ  ),ZV(#HZ  ),R2(#HZ  ),
     & YL1(#AZ ),XEE(#C ),YL(45,2),CN(#IZ ),EZ(#X),
     & XX(#HZ  ),YY(#HZ  ),ZZ(#HZ  ),XOT(#G ),YOT(#G ),ZOT(#G ),
     & AXX(#NZZ  ),AYY(#NZZ  ),AZZ(#NZZ  ),CHZ(#NZZ  ),KPL1(#HZ  ),
     & MN(#IZ ),NATOM(#IZ ),IATOM(#IZ ),NEQV(#HZ  ),KMAD,KDUM
      ICRD=4
      READ(ICRD,101) NN1,NN2,NN3,IPRT
      READ(ICRD,'(3F10.5)') XBL,YBL,ZBL
      READ(ICRD,'(3F10.5)') XB0,YB0,ZB0
      READ(ICRD,101) NPW,(NWV(I),I=1,NPW)
      IF (MSTP.EQ.2) THEN
       READ(ICRD,101) NDUM,(NWS(I),I=1,NPW)
      ELSE
       DO I=1,NPW
        IF (NWS(I).EQ.0) NWS(I)=1 
       END DO
      END IF
 
C     XB0,YB0,ZB0: Origin
C     XBL,YBL,ZBL: Length of Box 

   30 CONTINUE
      XP0=-XBL/2+XB0
      YP0=-YBL/2+YB0
      ZP0=-ZBL/2+ZB0
      DXP=XBL/(NN1-1)
      DYP=0.0D0
      DZP=0.0D0
      DX0=0.0D0
      DY0=YBL/(NN2-1)
      DZ0=0.0D0
 
      CALL POINGT(XP0,YP0,ZP0,DXP,DYP,DZP,DX0,DY0,DZ0,NN1,NN2,
     & NN3,YL1,XV,YV,ZV)
 
   10 RETURN
  101 FORMAT(16I5)
  102 FORMAT(3D20.10)
      END
C   ---------------------------------------------
C   SUBROUTINE POINGT TO GET MESH POINTS
C
      SUBROUTINE POINGT(XP0,YP0,ZP0,DXP,DYP,DZP,DX0,DY0,DZ0,NN1,NN2,
     & NN3,YL1,XV,YV,ZV)
       IMPLICIT REAL*8(A-H,O-Z)

      COMMON / IOCNTR/IFL1,IFL2,IFL3,IFL4,IFL5,IFL6,IFL7,IFL8,
     & ITSTR,JFL1,JFL2,JFL3,JFL4,JFL5,JFL6,JFL7,JFL8,
     & INPUT,IOUT,IPUN,IEIG,ITP,IYL,ISAV,IEXPC,JPRT,JPUN
      COMMON / CATCNT/PHM,TOLE,DEL,TOL1,REMV1,REMV2,REMV3,
     & LITERN,ICYCLE,IWRE,NDWAV,NDVR,NMAX,IPOP,NSCF,NSCC,
     & JLV1,JLV2,JLV3,LSY1,LSY2,LSY3,KSP1,KSP2,KSP3,MSTP
      COMMON / PROND/ ZN1(#X),DHD(#X),RS(#X),XALFA(#X),
     & ANUC(#X),RNUC(#X),RW(#X),VW(#X),JCORE(#X),KMAX(#X),
     & NIATOM(#X),NCO(#X),MSTAR(#X),KPLV(#X)
      COMMON / PROCNT/ EXALF,XFAC,TL,RIDO,VIDO,TOLM,
     & KKK2,NCYCLE,NTABPT, NMAXK,NAT,NDAT,NCORE,NATOMS,
     & NTYPES,NSYM,NAOS,NMOS,IIDO,LTIME,NNST,ITAPE,NTAPE,
     & MAOS,MISOS,IDOA,ITIME,NGRD,MSCF,MQ,MMOL(10),
     & ISYML(#M),JSYML(#M),NORB(#M),LORB(#M),MTITL(4,#M)
      COMMON / PROATM/ XVP(#HZ  ),YVP(#HZ  ),ZVP(#HZ  ),Z(#G ),
     & R0(#HZ  ),LMAXZ(#G ),NEQP(#HZ  ),NEQ(#G )
      COMMON / PRAMO/ EIGN1(#EZZ  ),LVAL(#AZ ),LOCA(#AZ ),NBPT(#AZ ),
     & KAT(#AZ ),MCOPT(#AZ ),MVAL(#AZ ),NPART(#EZZ  )
N     COMMON / PRMESH/ RHS(#LZ,#X),RHO(#LZ,#X),NAME(10,#X)
S     COMMON / PRMESH/ RHS(#LZ,#X),RHO(#LZ,#X,2),RHT(#LZ,#X),
S    & NAME(10,#X)
N     COMMON / CATPR/ TOHI(#X),ECO(#C ),XZM(#C ),DXZM(#C ),
N    & VHT(#LZ,#X),SNLO(#LZ,#C ),VHS(#LZ,#X)
S     COMMON / CATPR/ TOHI(#X),ECO(#C ,2),XZM(#C ,2),DXZM(#C ,2),
S    & VHT(#LZ,#X,2),SNLO(#LZ,#C ,2),VHS(#LZ,#X)
N     COMMON / CATOUT/SMX2(#BZZ   ),FMX1(#BZZ   ),EIEJ(#BZZ   ),
S     COMMON / CATOUT/SMX2(#BZZ   ,2),FMX1(#BZZ   ,2),EIEJ(#BZZ   ,2),
N    & EET(#AZ ),ELNO(#AZ ),ITITL(#AZ ),NTITL(#AZ ),
N    & NII(#AZ ),ISPIN(#AZ )
S    & EET(#AZ ,2),ELNO(#AZ ,2),ITITL(#AZ ,2),NTITL(#AZ ,2),
S    & NII(#AZ ,2),ISPIN(#AZ ,2)
      COMMON/CONT/ KPRT,IFW,ILW,NPW,MINJ,MAXJ,NWV(100),NWS(100),IPRT
      COMMON/CONT3D/XBL,YBL,ZBL
N     COMMON / / SMX1(#BZZ   ),BAS1(#AZ ),TBAS1(#AZ ),WAV11(#AZ ),
S     COMMON / / SMX1(#BZZ   ,2),BAS1(#AZ ),TBAS1(#AZ ),WAV11(#AZ ),
     & TWAV11(#AZ ),R2(#HZ  ),FNS(#C ),VNS(#X),YL(#AZ ),
N    & CN(#DZZ   ),KPL1(#HZ  ),IFL(2),NSY(#M)
S    & CN(#DZZ   ,2),KPL1(#HZ  ),IFL(2),NSY(#M)

      DIMENSION YL1(1),XV(1),YV(1),ZV(1)
      DIMENSION CHGD(1000000),POTD(1000000),WAVD(10,1000000)

      REWIND IEIG
      DO 10 IS=1,MSTP
      KK1=1
      DO 10 ISYM=1,NSYM
      IF(JSYML(ISYM).EQ.0) GO TO 10
      NSOS=NORB(ISYM)
      NSIZ=NSOS**2
      KK2=KK1+NSIZ-1
N     READ(IEIG) (CN(K),K=KK1,KK2)
S     READ(IEIG) (CN(K,IS),K=KK1,KK2)
      IF(IS.EQ.1) NSY(ISYM)=KK1
      KK1=KK2+1
   10 CONTINUE

      NCOUNT=0
      ZP0=ZP0-ZBL/(NN3-1)
      DO 250 N3=1,NN3
      XS=DXP+DX0
      YS=DYP+DY0
      ZS=DZP+DZ0
      ZP0=ZP0+ZBL/(NN3-1)
      write(*,30) N3,NN3,ZP0
   30 FORMAT(I3,' /',I3,'  Z=',F10.5)
      DO 250 N2=1,NN2
      XS=XS-DX0
      YS=YS-DY0
      ZS=ZS-DZ0
      XP=XP0-XS
      YP=YP0-YS
      ZP=ZP0-ZS
      DO 250 N1=1,NN1
      XP=XP+DXP
      YP=YP+DYP
      ZP=ZP+DZP
      APOT=0.0D0
      DO 130 I=1,NATOMS
      NQ = NEQP(I)
      XV(I)=XP-XVP(I)
      YV(I)=YP-YVP(I)
      ZV(I)=ZP-ZVP(I)
      RNUS=XV(I)**2+YV(I)**2+ZV(I)**2
      RNU =DSQRT(RNUS)
      R2(I)=RNU
      I1=I
      CALL KPLGET(NTABPT,NAT,I1,RNU,R0(I),KPLV(NQ),KPL1(I),RHS(1,NQ))
      IF( I.GT.NAT ) GO TO 130
      IF( RNU.LT.RS(NQ) ) GO TO 50
      DO 20 IA = 1,NAOS
      IF( LOCA(IA).NE.I ) GO TO 20
      YL1(IA)=0.0D0
   20 CONTINUE
      GO TO 130
   50 NLAB = LMAXZ(I)
      CALL YLMGET(RNU,XV(I),YV(I),ZV(I),NLAB,YL)
      CALL HARM(I1,LVAL,YL,LOCA,NAOS,YL1,MVAL)
  130 CONTINUE

C     WRITE(IYL) (R2(I),KPL1(I),I=1,NATOMS),(YL1(IA),IA=1,NAOS),APOT
C     READ(IYL)  (R2(I),KPL1(I),I=1,NATOMS),(YL(IA),IA=1,NAOS),APOT

      NCOUNT=NCOUNT+1
      CALL CPOTGS(CPOT,NATOMS,NEQP,R2,NTABPT,TOHI,KPL1,NTYPES,RHS,VHS)
      DO 27 IS=1,MSTP
S     CALL CHARB(CHAR,NATOMS,RHO(1,1,IS),R2,TOHI,RHS,KPL1,NTABPT,
N     CALL CHARB(CHAR,NATOMS,RHO,R2,TOHI,RHS,KPL1,NTABPT,
     & NTYPES,NEQP)
      IF (CHAR.LT.0.0D0) CHAR=0.0D0
      CALL XPOTG(NAT,R2,R0,NEQ,XALFA,CHAR,EXALF,XFAC,XPOT)
      POT=CPOT+XPOT+APOT
      CALL BASGET(NTABPT,NAT,NAOS,NEQ,LOCA,MSTAR,NCO,KPL1,MCOPT,
N    & R2,RS,BAS1,TBAS1,RHS,VHT,VNS,SNLO,FNS,YL1,ECO,NCORE,NTYPES)
S    & R2,RS,BAS1,TBAS1,RHS,VHT(1,1,IS),VNS,SNLO(1,1,IS),FNS,YL1,
S    & ECO(1,IS),NCORE,NTYPES)
      CALL SOGET(NSYM,NORB,KAT,NPART,EIGN1,BAS1,TBAS1,WAV11,TWAV11)
N     CALL VALG(NSYM,NORB,LORB,JSYML,CN,WAV11,BAS1,NII,ELNO,
S     CALL VALG(NSYM,NORB,LORB,JSYML,CN(1,IS),WAV11,BAS1,NII,ELNO,
     & CHRG,NSY,NMOS,IS)

      IF (MSTP.EQ.2) THEN
       IF (IS.EQ.1) THEN
        CHRGU=CHRG
        POTU=POT
       ELSE
        CHRG=CHGR+CHRGU
        POT=POT+POTU
       END IF
      END IF
    
      CHGD(NCOUNT)=CHRG
      POTD(NCOUNT)=POT
      DO IW=1,NPW
       IF (NWS(IW).EQ.IS) WAVD(IW,NCOUNT)=BAS1(NWV(IW))
      END DO
 
   27 CONTINUE

  250 CONTINUE

       WRITE(7,190) NN1,NN2,NN3,XBL,YBL,ZBL
       WRITE(8,190) NN1,NN2,NN3,XBL,YBL,ZBL
      DO N3=1,NN3
      DO N2=1,NN2
       IOUT=1+NN1*(N2-1)+NN1*NN2*(N3-1)
       IF (NN1.LE.71) THEN
        WRITE(7,200) (CHGD(I),I=IOUT,IOUT+NN1-1)
        WRITE(8,200) (POTD(I),I=IOUT,IOUT+NN1-1)
       ELSE
        WRITE(7,210) (CHGD(I),I=IOUT,IOUT+NN1-1)
        WRITE(8,210) (POTD(I),I=IOUT,IOUT+NN1-1)
       END IF
      END DO
      END DO

      DO IW=1,NPW
      CALL OPENWAVF(NWV(IW),NWS(IW))
      WRITE(15,190) NN1,NN2,NN3,XBL,YBL,ZBL
      DO N3=1,NN3
      DO N2=1,NN2
      IOUT=1+NN1*(N2-1)+NN1*NN2*(N3-1)
       IF (NN1.LE.71) THEN
        WRITE(15,200) (WAVD(IW,I),I=IOUT,IOUT+NN1-1)
       ELSE
        WRITE(15,210) (WAVD(IW,I),I=IOUT,IOUT+NN1-1)
       END IF
      END DO
      END DO
      CLOSE(UNIT=15)
      END DO

      RETURN
  190 FORMAT(3I5,3F10.5)
  200 FORMAT(100E14.6)
  210 FORMAT(10E14.6)

      RETURN
      END
C   --------------------------------------------------------
C   SUBROUTINE VALG TO GET M.O. AND CHARGE AT MESH POINT
C   NEED-NSYM,JSYML,NORB,NII,ELNO
C   GET-BAS=M.O.,CHRG=CHARGE DENSITY AT MESH POINT
C   CALL-PTWAV
C
      SUBROUTINE VALG(NSYM,NORB,LORB,JSYML,C,WAV11,BAS,NII,ELNO,CHRG,
     & NSY,NMOS,ISP)
       IMPLICIT REAL*8(A-H,O-Z)
      DIMENSION NORB(1),WAV11(1),BAS(1),C(1),ELNO(1),NII(1),LORB(1)
      DIMENSION JSYML(1),NSY(1)
      LS=1
      CHRG=0.0D0
      DO 20 IS=1,NSYM
      IF(JSYML(IS).EQ.0) GO TO 5
      NSOS=NORB(IS)
      KK1=NSY(IS)
    5 CONTINUE
      CALL PTWAV(C(KK1),WAV11(LS),BAS(LS),NSOS)
      LL=LS-1
      DO 10 I=1,NSOS
      MS=LORB(IS)+I+NMOS*(ISP-1)
      J=NII(MS)
      L=LL+I
      CH=ELNO(J)*BAS(L)**2
      CHRG=CHRG+CH
   10 CONTINUE
      LS=LS+NSOS
   20 CONTINUE
      RETURN
      END
C   -----------------------------------------
C   SUBROUTINE PTWAV TO GET M.O.=C(I,J)*S.O.
C   NEED-C(COE),W(S.0.)
C   GET-B(M.O.)
C
      SUBROUTINE PTWAV(C,W,B,N)
       IMPLICIT REAL*8(A-H,O-Z)
      DIMENSION C(1),W(1),B(1)
      K=0
      DO 10 L=1,N
      B(L)=0.0D0
      DO 10 I=1,N
      K=K+1
      B(L)=B(L)+C(K)*W(I)
   10 CONTINUE
      RETURN
      END

C   -----------------------------------------
      SUBROUTINE OPENWAVF(NWV,NWS)
       IMPLICIT REAL*8(A-H,O-Z)

        character*12 filename
        character*10 number
        character*1 fln1
        character*2 fln2
        character*3 fln3
        character*4 fln4
c
      DATA number/'1234567890'/
c
      je=1
      if (nwv.ge.  10) je=2
      if (nwv.ge. 100) je=3
      if (nwv.ge.1000) je=4

       k=nwv
      do j=1,je
       ii=int(k/10**((je-j)))
       jj=ii
       if (jj.eq.0) jj=10 
       if (j.eq.1) fln1=number(jj:jj) 
       if (j.eq.2) fln2=fln1//number(jj:jj) 
       if (j.eq.3) fln3=fln2//number(jj:jj) 
       if (j.eq.4) fln4=fln3//number(jj:jj) 
       k=k-ii*10**(je-j)
      end do
       if (je.eq.1.and.NWS.ne.2) filename='w'//fln1//'-3d.txt'
       if (je.eq.2.and.NWS.ne.2) filename='w'//fln2//'-3d.txt'
       if (je.eq.3.and.NWS.ne.2) filename='w'//fln3//'-3d.txt'
       if (je.eq.4.and.NWS.ne.2) filename='w'//fln4//'-3d.txt'
       if (je.eq.1.and.NWS.eq.2) filename='w'//fln1//'d-3d.txt'
       if (je.eq.2.and.NWS.eq.2) filename='w'//fln2//'d-3d.txt'
       if (je.eq.3.and.NWS.eq.2) filename='w'//fln3//'d-3d.txt'
       if (je.eq.4.and.NWS.eq.2) filename='w'//fln4//'d-3d.txt'

       open(unit=15,file=filename)

      RETURN
      END
