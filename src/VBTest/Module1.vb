﻿



Module Module1

    Public Declare Function Function1 Lib "NewUId.dll" (ByVal a As Integer, ByVal b As Integer) As Integer
    Public Declare Function JT_Layouts Lib "NewUId.dll" (ByVal src_file_path As String) As Integer


    Sub Main()
        Dim c As Integer
        c = Function1(1, 3)


        Dim s As String

        s = "F:\\PictureLayout\\真实测试.xml"
        JT_Layouts(s)

        MsgBox("Hello World")

    End Sub

End Module