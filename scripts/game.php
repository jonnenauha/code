#!/usr/bin/php
<?php
$ch = curl_init();

# login
curl_setopt ($ch, CURLOPT_URL, "http://triangle.lan/kintai/menu/entry.asp");
curl_setopt ($ch, CURLOPT_POST, 1);
curl_setopt ($ch, CURLOPT_POSTFIELDS, "user_ID=07081007&passwd=2240037&kai=4");
curl_setopt ($ch, CURLOPT_COOKIEJAR, "test.cookies");
curl_setopt ($ch, CURLOPT_COOKIEFILE, "test.cookies");
curl_exec ($ch);



#<option Selected Value='65'>出勤</option>
#<option          Value='66'>出勤（半休）</option>
#<option          Value='68'>出勤（フレックス）</option>
#<option          Value='71'>退勤</option>
#<option          Value='72'>退勤（半休）</option>
#<option          Value='74'>退勤（残業）</option>
#<option          Value='76'>退勤（フレックス）</option>
#<option          Value='79'>外出</option>
#<option          Value='81'>戻り</option>
#<option          Value='83'>早出</option>
#<option          Value='85'>休出</option>

#"ReSelSya_CD1=&ReSelSya_CD2=&ReSelSya_CD3=&ReSelSya_CD4=&ReSelSya_CD5=&SelBuName=&SelSyaName=&SelBunName=&SelSyName=&tyukei=&SYear=&EYear=&SMonth=&EMonth=&SDay=&EDay=&SDate=&EDate=&s_koumoku0=&s_koumoku1=&s_koumoku2=&s_koumoku3=&s_koumoku4=&s_koumoku5=&s_koumoku6=&s_koumoku7=&s_syounin0=&s_syounin1=&s_syounin2=&no=&skb=&sya_cd=&sin_date=&sin_kb=&maxno=&seliyyyymmdd=&selitime=&seliyyyy=&selimmdd=&deleteflg=&Wki_Place=&sya_cd_bak=&sin_date_bak=&sin_kb_bak=&seliyyyymmdd_bak=&seliyyyy_bak=&selimmdd_bak=&selitime_bak=&skb_bak=&PageNo=&kinds=&Det_sya_cd=&Det_date=&Det_inputdate=&Det_inputtime=&Wki_SyaCD=&Wki_date=&Wk_sYYYY=&Wk_sMM=&Wk_sDD=&Wk_iYYYY=&Wk_iMM=&Wk_iDD=&Wk_iDate=&Wk_iTime=&Wkl_s_syacd(1)=&Wkl_s_skb(1)=&Wkl_s_syyyy(1)=&Wkl_s_smmdd(1)=&Wkl_s_ssr(1)=&Wkl_s_syacd(2)=&Wkl_s_skb(2)=&Wkl_s_syyyy(2)=&Wkl_s_smmdd(2)=&Wkl_s_ssr(2)=&Wkl_s_syacd(3)=&Wkl_s_skb(3)=&Wkl_s_syyyy(3)=&Wkl_s_smmdd(3)=&Wkl_s_ssr(3)=&Wkl_s_syacd(4)=&Wkl_s_skb(4)=&Wkl_s_syyyy(4)=&Wkl_s_smmdd(4)=&Wkl_s_ssr(4)=&Wkl_s_syacd(5)=&Wkl_s_skb(5)=&Wkl_s_syyyy(5)=&Wkl_s_smmdd(5)=&Wkl_s_ssr(5)=&"

# change hours
curl_setopt ($ch, CURLOPT_URL, "http://triangle.lan/kintai/menu/sinsei/touroku.asp");
curl_setopt ($ch, CURLOPT_POST, 1);
curl_setopt ($ch, CURLOPT_POSTFIELDS, "mode=NEW&YYYY=2008&MM=08&DD=01&kisya_hour=09&kisya_min=30&kinmusyurui=65&SelSya_CD1=07081011&SelSya_CD2=01fujimo&SelSya_CD3=02kaneko");
curl_setopt ($ch, CURLOPT_COOKIEJAR, "test.cookies");
curl_setopt ($ch, CURLOPT_COOKIEFILE, "test.cookies");
curl_exec ($ch);


curl_close ($ch);
?>
