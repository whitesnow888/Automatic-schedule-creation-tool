'定数の設定
Const D_CNT = 43 '講習の日数
Const D_KOMA = 5 '一日のコマ数

Sub ピンクにする(生徒, 教科)
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    Dim SH5 As Worksheet
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    Set SH5 = Worksheets("生徒日程")    'ワークシートを選択
    
    For A = 4 To SH2.Cells(1, 2) + 3
        If SH1.Cells(A, 教科) = "1" Then
            For B = 0 To D_CNT - 1
                For C = 0 To D_KOMA - 1
                    '文字が入ってなく　かつ　灰色じゃなければ　ピンク色にする　一つ目の枠
                    'If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A) = "") And
                    If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex <> 15) And _
                       (SH5.Cells(生徒, ((B * D_KOMA) + C + 3)) <> "0") Then
                        SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex = 38
                    End If
                    '文字が入ってなく　かつ　灰色じゃなければ　ピンク色にする　二つ目の枠
                    'If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A) = "") And
                    If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex <> 15) And _
                       (SH5.Cells(生徒, ((B * D_KOMA) + C + 3)) <> "0") Then
                        SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex = 38
                    End If
                Next
            Next
        End If
    Next
End Sub

Sub 同日時同時限ピンクを戻す(名前)
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    For A = 4 To SH2.Cells(1, 2) + 3
        For B = 0 To D_CNT - 1
            For C = 0 To D_KOMA - 1
                'ピンクのセルを色なしに戻す　一つ目の枠
                If (InStrRev(SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A), 名前) <> 0 Or _
                    InStrRev(SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A), 名前) <> 0) Then
                    For D = 4 To SH2.Cells(1, 2) + 4
                        If SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), D).Interior.ColorIndex <> 15 Then
                            SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), D).Interior.ColorIndex = XlName
                            SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), D).Interior.ColorIndex = XlName
                        End If
                    Next
                End If
            Next
        Next
    Next
End Sub

Sub 名前を赤にする(名前)
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    For A = 4 To SH2.Cells(1, 2) + 3
        For B = 0 To D_CNT - 1
            For C = 0 To D_KOMA - 1
                'ピンクのセルを色なしに戻す　一つ目の枠
                If (InStrRev(SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A), 名前)) <> 0 Then
                    SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex = 3
                End If
                If (InStrRev(SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A), 名前)) <> 0 Then
                    SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex = 3
                End If
            Next
        Next
    Next
End Sub

Sub ピンクを戻す()
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    For A = 4 To SH2.Cells(1, 2) + 4
        For B = 0 To D_CNT - 1
            For C = 0 To D_KOMA - 1
                'ピンクのセルを色なしに戻す　一つ目の枠
                If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex = 38) Then
                    SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex = XlName
                End If
                'ピンクのセルを色なしに戻す　二つ目の枠
                If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex = 38) Then
                    SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex = XlName
                End If
                'ピンクのセルを色なしに戻す　一つ目の枠
                If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex = 3) Then
                    SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex = XlName
                End If
                'ピンクのセルを色なしに戻す　二つ目の枠
                If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex = 3) Then
                    SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex = XlName
                End If
            Next
        Next
    Next
End Sub

Function 学年を調べる(横)
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    If (29 <= 横) Then
        学年を調べる = "小"
    ElseIf (24 <= 横) Then
        学年を調べる = "中1"
    ElseIf (19 <= 横) Then
        学年を調べる = "中2"
    ElseIf (14 <= 横) Then
        学年を調べる = "中3"
    ElseIf (12 <= 横) Then
        学年を調べる = "高1"
    ElseIf (10 <= 横) Then
        学年を調べる = "高2"
    ElseIf (8 <= 横) Then
        学年を調べる = "高3"
    ElseIf (3 <= 横) Then
        学年を調べる = "高"
    End If

End Function

Sub 講師不在を灰色()
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    For A = 1 To SH2.Cells(1, 2)
        For B = 1 To (D_KOMA * D_CNT)
            If SH2.Cells((3 + A), (2 + B)) = "0" Then       '0の場合は休み
                SH3.Cells(3 + ((B - 1) * 2) + ((B - 1) \ D_KOMA), A + 3).Interior.ColorIndex = 15   '背景色を灰色に設定
                SH3.Cells(4 + ((B - 1) * 2) + ((B - 1) \ D_KOMA), A + 3).Interior.ColorIndex = 15   '背景色を灰色に設定
            Else
                SH3.Cells(3 + ((B - 1) * 2) + ((B - 1) \ D_KOMA), A + 3).Interior.ColorIndex = XlName   '背景色をなしに設定
                SH3.Cells(4 + ((B - 1) * 2) + ((B - 1) \ D_KOMA), A + 3).Interior.ColorIndex = XlName   '背景色をなしに設定
            End If
        Next
    Next
End Sub


Sub 教科別残り空き数()
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    Dim Cnt As Integer
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    For D = 3 To 33
        Cnt = 0
        For A = 4 To SH2.Cells(1, 2) + 4
            If SH1.Cells(A, D) = "1" Then
            For B = 0 To D_CNT - 1
                For C = 0 To D_KOMA - 1
                    'ピンクのセルを色なしに戻す　一つ目の枠
                    If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Value = "") And _
                       (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex <> 15) Then
                        Cnt = Cnt + 1
                    End If
                    If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Value = "") And _
                       (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex <> 15) Then
                        Cnt = Cnt + 1
                    End If
                Next
            Next
            End If
        Next
        SH1.Cells(1, D) = Cnt
    Next
    教科別余裕コマ数
End Sub

Sub 教師別残り空き数()
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    Dim Cnt As Integer
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    
    For A = 4 To SH2.Cells(1, 2) + 3
        Cnt = 0
        For B = 0 To D_CNT - 1
            For C = 0 To D_KOMA - 1
                'ピンクのセルを色なしに戻す　一つ目の枠
                If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Value = "") And _
                   (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2), A).Interior.ColorIndex <> 15) Then
                    Cnt = Cnt + 1
                End If
                If (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Value = "") And _
                   (SH3.Cells(3 + (B * (D_KOMA * 2 + 1)) + (C * 2 + 1), A).Interior.ColorIndex <> 15) Then
                    Cnt = Cnt + 1
                End If
            Next
        Next
        SH1.Cells(A, 34) = Cnt
    Next

End Sub

Sub 教科別余裕コマ数()
    Dim SH1 As Worksheet
    Dim SH2 As Worksheet
    Dim SH3 As Worksheet
    Dim SH4 As Worksheet
    Dim SH6 As Worksheet
    Dim Cnt As Integer
    
    Set SH1 = Worksheets("講師教科")    'ワークシートを選択
    Set SH2 = Worksheets("講師日程")    'ワークシートを選択
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    Set SH6 = Worksheets("生徒入力用")    'ワークシートを選択
    
    For A = 3 To 33
        Cnt = 0
        For B = 0 To SH6.Cells(1, 2) - 1
            If (SH4.Cells(B + 4, A) <> "") Then
                Cnt = SH4.Cells(B + 4, A) + Cnt
            End If
        Next
        SH4.Cells(1, A) = SH1.Cells(1, A) - Cnt
    Next

End Sub

Function 生徒番号を探す(セル文字列)
    Dim SH4 As Worksheet
    Dim SH6 As Worksheet
    Dim Cnt As Integer
    
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択
    Set SH6 = Worksheets("生徒入力用")    'ワークシートを選択
    
    For A = 0 To SH6.Cells(1, 2) - 1
        If ((InStrRev(セル文字列, SH4.Cells(A + 4, 2))) <> 0) And _
           (SH4.Cells(A + 4, 2) <> "") Then
            生徒番号を探す = A + 4
        End If
    Next
End Function

Function 教科番号を探す(セル文字列)
    Dim SH4 As Worksheet
    Dim Cnt As Integer
    
    Set SH4 = Worksheets("生徒教科")    'ワークシートを選択

    If (InStrRev(セル文字列, "高国")) <> 0 Then
        教科番号を探す = 3
    ElseIf (InStrRev(セル文字列, "高物")) <> 0 Then
        教科番号を探す = 4
    ElseIf (InStrRev(セル文字列, "高生")) <> 0 Then
        教科番号を探す = 5
    ElseIf (InStrRev(セル文字列, "高化")) <> 0 Then
        教科番号を探す = 6
    ElseIf (InStrRev(セル文字列, "高社")) <> 0 Then
        教科番号を探す = 7
    ElseIf (InStrRev(セル文字列, "高3英")) <> 0 Then
        教科番号を探す = 8
    ElseIf (InStrRev(セル文字列, "高3数")) <> 0 Then
        教科番号を探す = 9
    ElseIf (InStrRev(セル文字列, "高2英")) <> 0 Then
        教科番号を探す = 10
    ElseIf (InStrRev(セル文字列, "高2数")) <> 0 Then
        教科番号を探す = 11
    ElseIf (InStrRev(セル文字列, "高1英")) <> 0 Then
        教科番号を探す = 12
    ElseIf (InStrRev(セル文字列, "高1数")) <> 0 Then
        教科番号を探す = 13
    ElseIf (InStrRev(セル文字列, "中3英")) <> 0 Then
        教科番号を探す = 14
    ElseIf (InStrRev(セル文字列, "中3数")) <> 0 Then
        教科番号を探す = 15
    ElseIf (InStrRev(セル文字列, "中3国")) <> 0 Then
        教科番号を探す = 16
    ElseIf (InStrRev(セル文字列, "中3理")) <> 0 Then
        教科番号を探す = 17
    ElseIf (InStrRev(セル文字列, "中3社")) <> 0 Then
        教科番号を探す = 18
    ElseIf (InStrRev(セル文字列, "中2英")) <> 0 Then
        教科番号を探す = 19
    ElseIf (InStrRev(セル文字列, "中2数")) <> 0 Then
        教科番号を探す = 20
    ElseIf (InStrRev(セル文字列, "中2国")) <> 0 Then
        教科番号を探す = 21
    ElseIf (InStrRev(セル文字列, "中2理")) <> 0 Then
        教科番号を探す = 22
    ElseIf (InStrRev(セル文字列, "中2社")) <> 0 Then
        教科番号を探す = 23
    ElseIf (InStrRev(セル文字列, "中1英")) <> 0 Then
        教科番号を探す = 24
    ElseIf (InStrRev(セル文字列, "中1数")) <> 0 Then
        教科番号を探す = 25
    ElseIf (InStrRev(セル文字列, "中1国")) <> 0 Then
        教科番号を探す = 26
    ElseIf (InStrRev(セル文字列, "中1理")) <> 0 Then
        教科番号を探す = 27
    ElseIf (InStrRev(セル文字列, "中1社")) <> 0 Then
        教科番号を探す = 28
    ElseIf (InStrRev(セル文字列, "小英")) <> 0 Then
        教科番号を探す = 29
    ElseIf (InStrRev(セル文字列, "小数")) <> 0 Then
        教科番号を探す = 30
    ElseIf (InStrRev(セル文字列, "小国")) <> 0 Then
        教科番号を探す = 31
    ElseIf (InStrRev(セル文字列, "小算国")) <> 0 Then
        教科番号を探す = 32
    ElseIf (InStrRev(セル文字列, "小算英")) <> 0 Then
        教科番号を探す = 33
    End If
End Function

Function 科目(文字列)
    If (InStrRev(文字列, "英")) <> 0 Then
        If (InStrRev(文字列, "算")) <> 0 Then
            科目 = "算 英"
        Else
            科目 = "英 語"
        End If
    ElseIf (InStrRev(文字列, "数")) <> 0 Then
        If (InStrRev(文字列, "小")) <> 0 Then
            科目 = "算 数"
        Else
            科目 = "数 学"
        End If
    ElseIf (InStrRev(文字列, "国")) <> 0 Then
        If (InStrRev(文字列, "算")) <> 0 Then
            科目 = "算 国"
        Else
            科目 = "国 語"
        End If
    ElseIf (InStrRev(文字列, "理")) <> 0 Then
        科目 = "理 科"
    ElseIf (InStrRev(文字列, "社")) <> 0 Then
        科目 = "社 会"
    ElseIf (InStrRev(文字列, "物")) <> 0 Then
        科目 = "物 理"
    ElseIf (InStrRev(文字列, "化")) <> 0 Then
        科目 = "化 学"
    ElseIf (InStrRev(文字列, "生")) <> 0 Then
        科目 = "生 物"
    End If
End Function

Sub 学年入力()
    Dim SH3 As Worksheet
    Dim SH7 As Worksheet
    
    Set SH3 = Worksheets("全日程")    'ワークシートを選択
    Set SH7 = Worksheets("生徒用スケ")    'ワークシートを選択
    
    If (InStrRev(SH3.Cells(4, 1), "小")) <> 0 Then
        SH7.Cells(1, 2) = "小 学"
    ElseIf (InStrRev(SH3.Cells(4, 1), "中")) <> 0 Then
        SH7.Cells(1, 2) = "中 学"
    ElseIf (InStrRev(SH3.Cells(4, 1), "高")) <> 0 Then
        SH7.Cells(1, 2) = "高 校"
    End If
    
    If (InStrRev(SH3.Cells(4, 1), "１")) <> 0 Then
        SH7.Cells(1, 3) = "１ 年"
    ElseIf (InStrRev(SH3.Cells(4, 1), "２")) <> 0 Then
        SH7.Cells(1, 3) = "２ 年"
    ElseIf (InStrRev(SH3.Cells(4, 1), "３")) <> 0 Then
        SH7.Cells(1, 3) = "３ 年"
    ElseIf (InStrRev(SH3.Cells(4, 1), "４")) <> 0 Then
        SH7.Cells(1, 3) = "４ 年"
    ElseIf (InStrRev(SH3.Cells(4, 1), "５")) <> 0 Then
        SH7.Cells(1, 3) = "５ 年"
    ElseIf (InStrRev(SH3.Cells(4, 1), "６")) <> 0 Then
        SH7.Cells(1, 3) = "６ 年"
    End If
End Sub

