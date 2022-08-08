const char MAIN_page_1[] PROGMEM = R"=====(
<html>

<head>
    <title>Piof</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/flatpickr/dist/flatpickr.min.css">
    <script src="https://cdn.jsdelivr.net/npm/flatpickr"></script>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap" rel="stylesheet">
    <style>
        * {
            font-family: 'Press Start 2P', cursive;
        }

        html {
            height: 100%;
        }

        body {
            background-size: 100% 100%;
            margin: 0;
        }

        #mainContainer {
            position: absolute;
            text-align: center;
            width: 100%;
            height: 100%;

        }


        .inner_info {
            width: 100px;
            height: 100px;
            float: left;
            margin: 10px;
        }

        #temp {
            margin-left: 41%;
        }

        #start_btn {
            width: 110px;
            height: 60px;
            background-color: greenyellow;
            border-radius: 10%;
            border-color: silver;
            font-size: 18px;
            text-align: center;
            font-family: 'Press Start 2P', cursive;
        }

        #start_btn:hover {
            background-color: yellowgreen;
            border-color: black;

        }


        #calendar {
            margin: 0 auto;
            width: 300px;
            padding: 5px
        }


        .radioDate {
            opacity: 0;
            position: fixed;
            width: 0;
        }


        .radioDate_label {
            display: inline-block;
            background-color: greenyellow;
            padding: 10px 20px;
            font-family: 'Press Start 2P', cursive;
            font-size: 16px;
            border: 2px solid silver;
            border-radius: 4px;
        }

        .radioDate:checked+.radioDate_label {
            background-color: yellowgreen;
            border-color: black;
        }


        .radioDate:focus+.radioDate_label {
            border: 2px dashed #444;
        }

        .radio-toolbar label:hover {
            background-color: #dfd;
        }

        #time {
            width: 80px;
            height: 40px;
            text-align: center;
        }

        .sendBtn {
            background-color: #ffc21c;
            border: solid 1px transparent;
            color: #fff;
            border-radius: 40px;
            border-width: 1px;
        }


        #date {
            border-radius: 40px;
        }

        #dateManger {
            width: 100px;
            height: 100px;
            float: left;
            padding: 20px;
            margin: 10px;
        }

        .flatpickr-month {
            /*font-family: 'Arial';*/
            font-size: 10px;


        }

        .flatpickr-current-month .flatpickr-monthDropdown-months {
            left: auto;
            float: left;
            width: 149px;
        }

        #title {
            font-size: 50px;
            padding: 10px;
            margin: 10px;
        }

        #dtw {
            width: 140px;
            height: 40px;
            font-size: 14px;
            border: 2px solid silver;
            border-radius: 4px;
            /*text-align: center;*/
        }

        #sendBtnContainer {
            position: relative;
            width: 20px;
            height: 90px;
            left: 230px;
        }

        #calendarContainer .flatpickr-input {
            width: 225px;
            position: relative;
            left: -40px;
        }

        #startForm {
            padding: 25px;
        }
    </style>

    <script>
        function refresh() {
            location.reload();
        }
    </script>

</head>

<body>

    <div id="mainContainer">
        <img src="https://play-lh.googleusercontent.com/DfEB91KZs7JrOIIufWJHJvZJx7bccODKCDjFD1FH4uS7jPZ2sLl-y-3K27u-GMjydg"
            id="arduinoIcon" alt="Icon" width="100" height="100">
        <p id="title">PIOF</p>
        <div id="InfoMeteo" class="container">
            <div id="temp" class="inner_info">
                <p id="tempText">0 &#8451</p>
            </div>
            <div id="meteoIcon" class="inner_info">
                <img src="" id="meteoImage" width="100" height="100">
            </div>
            <div id="hum" class="inner_info">
                <p id="humText">0</p>
            </div>
        </div>
        <div id="calendar_Input" class="container" id="dateManger">
            <form method="post" id="calendar">
                <div id="sendBtnContainer" class="calendarChild">
                    <input type="submit" id="sendDate" class="sendBtn">
                </div>
                <div id="calendarContainer" class="calendarChild">
                    <input type="datetime-local" id="date" name="date" placeholder="Select Date" onclick="refresh()">
                </div>


                <script>
                    flatpickr("input[type=datetime-local]", {

                        inline: true,
                        enableTime: true,
                        dateFormat: "Y-m-d H:i",
                        minDate: new Date(),
                        allowInput: false,
                    })
                </script>

            </form>
        </div>
        <div class="container" id="startContainer">
            <form method="post" id="startForm">
                <input type="submit" value="START" name="start" id="start_btn" onclick="refresh()" />
            </form>
        </div>
        <div class="container">
            <form method="post">
                <input type="datetime-local" id="time" name="time" placeholder="Time">
                <script>
                    flatpickr("input[type=datetime-local]", {
                        enableTime: true,
                        noCalendar: true,
                        dateFormat: "H:i",
                        time_24hr: true
                    })

                </script>
                <input type="radio" id="daily" name="modality" value="daily" class="radioDate" onclick="showWeek()"
                    checked>
                <label for="daily" class="radioDate_label">Daily</label>

                <input type="radio" id="weekly" name="modality" value="weekly" class="radioDate" onclick="showWeek()">
                <label for="weekly" class="radioDate_label" id="weeklyLabel">Weekly</label>
                <select name="dayOfTheWeek" id="dtw" style='display:none'>
                    <option value="2">Monday</option>
                    <option value="3">Tuesday</option>
                    <option value="4">Wednesday</option>
                    <option value="5">Thursday</option>
                    <option value="6">Friday</option>
                    <option value="7">Saturday</option>
                    <option value="1">Sunday</option>
                </select>

                <input type="submit" id="sendTime" class="sendBtn" onclick="refresh()">
            </form>
            <script>
                const weekChkbx = document.getElementById('weekly');
                function showWeek() {
                    if (weekChkbx.checked) {
                        document.getElementById("weeklyLabel").style.display = "none";
                        document.getElementById("dtw").style.display = "inline-block";

                    } else {
                        document.getElementById("weeklyLabel").style.display = "inline-block";
                        document.getElementById("dtw").style.display = "none";
                    }
                }
            </script>
        </div>
    </div>
    <script>
        
)=====";
