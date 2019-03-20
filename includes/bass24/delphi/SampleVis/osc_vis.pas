unit osc_vis;
{ Oscilloscope Visualyzation by Alessandro Cappellozza
  version 0.8 05/2002
  http://digilander.iol.it/Kappe/audioobject
}

interface
  uses Windows, Dialogs, Graphics, SysUtils, CommonTypes, Classes;

 type TOcilloScope = Class(TObject)
    private
      VisBuff : TBitmap;
      BackBmp : TBitmap;

      BkgColor : TColor;
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
     property Offset : Integer read ScopeOff write ScopeOff;
     property Pen  : TColor read PenColor write PenColor;
     property Mode : Integer read DrawType write DrawType;
     property Res  : Integer read DrawRes write DrawRes;
     property FrameClear : Boolean read FrmClear write FrmClear;
  end;

 var OcilloScope : TOcilloScope;

implementation

     Constructor TOcilloScope.Create(Width, Height : Integer);
      begin
        VisBuff := TBitmap.Create;
        BackBmp := TBitmap.Create;

          VisBuff.Width := Width;
          VisBuff.Height := Height;
          BackBmp.Width := Width;
          BackBmp.Height := Height;

          BkgColor := clBlack;
          ScopeOff := 50;
          PenColor := clWhite;
          DrawType := 0;
          DrawRes  := 1;
          FrmClear := True;
          UseBkg := False;
      end;

     procedure TOcilloScope.SetBackGround (Active : Boolean; BkgCanvas : TGraphic);
      begin
        UseBkg := Active;
        BackBmp.Canvas.Draw(0, 0, BkgCanvas);
      end;

     procedure TOcilloScope.Draw(HWND : THandle; WaveData : TWaveData; X, Y : Integer);
        var i, YPos : LongInt; R, L : SmallInt;
       begin
       if FrmClear then begin
         VisBuff.Canvas.Pen.Color := BkgColor;
         VisBuff.Canvas.Brush.Color := BkgColor;
         if UseBkg then
           BitBlt(VisBuff.Canvas.Handle,         // Destination
                  0, 0,                          // X, Y (target pos)
                  VisBuff.Width, VisBuff.Height, // Size to copy
                  BackBmp.Canvas.handle,         // Source
                  0, 0,                          // X, Y (source pos)
                  SrcCopy)                       // plain copy
         else
           VisBuff.Canvas.Rectangle(0, 0, VisBuff.Width, VisBuff.Height) // only if no background
       end;

        VisBuff.Canvas.Pen.Color := PenColor;
            R :=  SmallInt(LOWORD(WaveData[0]));
            L := SmallInt(HIWORD(WaveData[0]));
            YPos := Trunc(((R + L) / (2 * 65535)) * ScopeOff) ;
            VisBuff.Canvas.MoveTo(X , Y + YPos);

         for i := 1 to 256 do begin
            R := SmallInt(Loword(WaveData[i * DrawRes]));
            L := SmallInt(HIword(WaveData[i * DrawRes]));
            YPos := Trunc(((R + L) / (2 * 65535)) * ScopeOff) ;

              case DrawType of
                0 : VisBuff.Canvas.lineto(X + i, Y + YPos);

                1 : begin
                      VisBuff.Canvas.MoveTo(X + i, Y);
                      VisBuff.Canvas.lineto(X + i, Y + YPos);
                    end;

                2 : VisBuff.Canvas.Pixels[X + i,  Y + YPos] := PenColor;
              end;
         end;

          BitBlt(HWND, 0, 0, VisBuff.Width, VisBuff.Height, VisBuff.Canvas.Handle, 0, 0, srccopy)
       end;
end.

