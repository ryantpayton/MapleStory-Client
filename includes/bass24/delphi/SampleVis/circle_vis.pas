unit circle_vis;
{ Circle Visualyzation by Alessandro Cappellozza
  version 0.5 05/2002
  http://digilander.iol.it/Kappe/audioobject
}

interface
  uses Windows, Dialogs, Graphics, SysUtils, CommonTypes, Classes;

 type TCircleScope = Class(TObject)
    private
      VisBuff : TBitmap;
      BackBmp : TBitmap;

      BkgColor : TColor;
      ScopeRad : Integer;
      ScopeOff : Integer;
      PenColor : TColor;
      DrawType : Integer;
      DrawRes  : Integer;
      FrmClear : Boolean;
      UseBkg   : Boolean;

    public
     Constructor Create (Width, Height : Integer);
     procedure Draw(HWND : THandle; WaveData : TWaveData; X, Y : Integer);
     procedure SetBackGround (Active : Boolean; BkgCanvas : TGraphic);

     property BackColor : TColor read BkgColor write BkgColor;
     property Radius : Integer read ScopeRad write ScopeRad;
     property Offset : Integer read ScopeOff write ScopeOff;
     property Pen  : TColor read PenColor write PenColor;
     property Mode : Integer read DrawType write DrawType;
     property Res  : Integer read DrawRes write DrawRes;
     property FrameClear : Boolean read FrmClear write FrmClear;
  end;

 var CircleScope : TCircleScope;

implementation

     Constructor TCircleScope.Create(Width, Height : Integer);
      begin
        VisBuff := TBitmap.Create;
        BackBmp := TBitmap.Create;

          VisBuff.Width := Width;
          VisBuff.Height := Height;
          BackBmp.Width := Width;
          BackBmp.Height := Height;

          BkgColor := clBlack;
          ScopeRad := 30;
          ScopeOff := 30;
          PenColor := clWhite;
          DrawType := 0;
          DrawRes  := 2;
          FrmClear := True;
          UseBkg := False;
      end;

     procedure TCircleScope.SetBackGround (Active : Boolean; BkgCanvas : TGraphic);
      begin
        UseBkg := Active;
        BackBmp.Canvas.Draw(0, 0, BkgCanvas);
      end;

     procedure TCircleScope.Draw(HWND : THandle; WaveData : TWaveData; X, Y : Integer);
        var i, Rd : Integer; Angle : Single; R, L : SmallInt;
       begin
       if FrmClear then begin
        VisBuff.Canvas.Pen.Color := BkgColor;
        VisBuff.Canvas.Brush.Color := BkgColor;
        VisBuff.Canvas.Rectangle(0, 0, VisBuff.Width, VisBuff.Height);
         if UseBkg then VisBuff.Canvas.CopyRect(Rect(0, 0, BackBmp.Width, BackBmp.Height), BackBmp.Canvas, Rect(0, 0, BackBmp.Width, BackBmp.Height));
       end;

        VisBuff.Canvas.Pen.Color := PenColor;
              R := LOWORD(WaveData[0]);
              L := HIWORD(WaveData[0]);
              Rd := Trunc(((R + L) / (2 * 65535)) * ScopeOff) + ScopeRad;
              VisBuff.Canvas.MoveTo(X, Y + Rd);

         for i := 1 to 254 do begin
             Angle := (i /256) * (6.28);
              R := LOWORD(WaveData[i * DrawRes]);
              L := HIWORD(WaveData[i * DrawRes]);
              Rd := Trunc(((R + L) / (2 * 65535)) * ScopeOff) + ScopeRad;

              case DrawType of
                0 : VisBuff.Canvas.lineto(X + Trunc(sin(Angle) * Rd), Y + Trunc(cos(Angle) * Rd));

                1 : begin
                      VisBuff.Canvas.MoveTo(X, Y);
                      VisBuff.Canvas.lineto(X + Trunc(sin(Angle) * Rd), Y + Trunc(cos(Angle) * Rd));
                    end;

                2 : VisBuff.Canvas.Pixels[X + Trunc(sin(Angle) * Rd), Y + Trunc(cos(Angle) * Rd)] := PenColor;
              end;
         end;

              R := LOWORD(WaveData[0]);
              L := HIWORD(WaveData[0]);
              Rd := Trunc(((R + L) / (2 * 65535)) * ScopeOff) + ScopeRad;
              VisBuff.Canvas.lineto(X, Y + Rd);

          BitBlt(HWND, 0, 0, VisBuff.Width, VisBuff.Height, VisBuff.Canvas.Handle, 0, 0, srccopy)
       end;
end.
