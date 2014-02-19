// ==UserScript==
// @name           jwbinfosys reformer
// @author         pro
// @description    Change styles and add some useful features for jwbinfosys
// @include        http://10.202.78.12/xscj.aspx*
// @include        http://10.202.78.12/xscj.aspx*
// @include        http://10.202.78.12/default2.aspx*
// @include        http://jwbinfosys.zju.edu.cn/xscj.aspx*
// @include        http://jwbinfosys.zju.edu.cn/default2.aspx*
// @include        http://10.202.78.12/default2.aspx*
// @include        http://jwbinfosys.zju.edu.cn/xscj.aspx*
// ==/UserScript==

function xpath(expr)
{
    return document.evaluate(
        expr,
        document,
        null,
        XPathResult.ORDERED_NODE_SNAPSHOT_TYPE,
        null
    );
}

function $(id)
{
    return document.getElementById(id);
}

if (typeof GM_addStyle === "undefined")
    GM_addStyle = function(css) {
        var sty = document.createElement("style");
        
        sty.setAttribute("type", "text/css");
        style.innerHTML = css;

        document.getElementsByTagName("head")[0].appendChild(sty);
    }

function doCalcGPA()
{
    // Using xpath to select the column of course credit?
    var x = xpath("//table[@id='DataGrid1']//tr/td[4]");

    var totalCredit = 0; // Total credit
    var totalZju = 0;    // Total credit*pg
    var totalAbroad = 0;    // Total credit*pg, stydu abroad

    var grade, credit, gpZju, gpAbroad, elem;

    // Skip first result, it is the table head row
    for (var i = 1, len = x.snapshotLength; i < len; i++) {
        elem = x.snapshotItem(i);
        credit = parseFloat(elem.innerHTML); // Credit
        grade = elem.previousElementSibling.innerHTML; // Grade

        if (grade.match("放弃"))
            continue;

        gpZju = parseFloat(elem.nextElementSibling.innerHTML);

        if (grade >= 85)
            gpAbroad = 4.0;
        else if (grade < 60)
            gpAbroad = 0;
        else if (grade.match("优")) // 90
            gpAbroad = 4.0;
        else if(grade.match("良")) // 80
            gpAbroad = 3.5;
        else if(grade.match("中")) // 70
            gpAbroad = 2.5;
        else if(grade.match("及格")) // 60
            gpAbroad = 1.5;
        else if(grade.match("不及格")) // < 60
            gpAbroad = 0;
        else if(grade.match("合格")) // 80
            gpAbroad = 3.5;
        else if(grade.match("不合格")) // 0
            gpAbroad = 0;
        else
            gpAbroad = 4.0 - (85 - grade) / 10; 

        var currentRow = elem.parentNode;
        var color = 245;

        if (gpZju >= 1.5)
            color = 245 - parseInt((gpZju-1.5) * 70);

        color = "#" + ("0" + color.toString(16)).slice(-2) + "CDDC";
        currentRow.setAttribute('style', 'background-color:'+color);

        totalZju += credit*gpZju;
        totalAbroad += credit*gpAbroad;
        totalCredit += credit;
    }

    return {
        "totalcredit": totalCredit,
        "gpaZju": Math.round(totalZju*100/totalCredit)/100,
        "gpaAbroad": Math.round(totalAbroad*100/totalCredit)/100
    };
}

function showGPAs(results)
{
    var dialog = document.createElement("div");

    dialog.id = "gpas";
    dialog.innerHTML = ("<p>总学分为：<span class='bold'>%totalcredit%</span></p><p>GPA(校内算法): " + 
        "<span class='bold'>%gpaZju%</span></p><p>GPA(出国算法): <span class='bold'>%gpaAbroad%</span></p>")
        .replace(/%.*?%/g, function(tag) {
        return results[tag.slice(1,-1)];
    });

    document.getElementsByClassName("indexmid")[0].appendChild(dialog);
}

function load()
{
    if (location.href.indexOf("default2.aspx") != -1) { // Login page
        // Fix the style
        GM_addStyle("\
            #Textbox3 {width:65px !important;margin-right:5px !important;}\
            img[src*='CheckCode.aspx'] { height: 20px !important; }\
            #RadioButtonList1 { white-space: nowrap !important;}\
            #RadioButtonList1 label {margin-right: -8px !important;}\
            #RadioButtonList1 td:last-of-type label {margin-right: 2px !important;}\
        ")

        // Press enter to login
        $("Textbox3").setAttribute("onkeydown", "if(event.keyCode == 13) __doPostBack('Button1', '');");
    } else {
        var results = doCalcGPA();

        GM_addStyle("\
            .indexmid {position: relative !important;}\
            #gpas { border: 1px solid #ccc !important;\
            position: absolute !important; right: 40px !important; top: 180px !important;\
            text-align: left; padding: 10px !important; background-color: #fefefe !important;}\
            #gpas .bold { font-weight: bold !important; color: red !important;}\
        ");

        showGPAs(results);
    }
}

load();
