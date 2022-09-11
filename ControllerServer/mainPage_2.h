

const char MAIN_page_2[] PROGMEM = R"=====(
   
     switch (w) {
            case "sunny":
                console.log("sunny");
                document.getElementById("meteoImage").src = "https://art.pixilart.com/sr2e93d4b8611e4.png";
                document.body.style.backgroundImage = "url('https://art.pixilart.com/sr2cfecadbd20cf.png')";
                break;
            case "rain":
                document.getElementById("meteoImage").src = "https://art.pixilart.com/sr2c77f834698bf.png";
                document.body.style.backgroundImage = "url('https://art.pixilart.com/sr24c3ddf81802f.png')";
                break;
            case "storm":
                document.getElementById("meteoImage").src = "https://art.pixilart.com/sr225d6b0e17334.png";
                document.body.style.backgroundImage = "url('https://art.pixilart.com/sr2c911dd2ab4f7.png')";
                break;
            case "cloudy":
                document.getElementById("meteoImage").src = "https://art.pixilart.com/sr2ec1d14653a99.png";
                document.body.style.backgroundImage = "url('https://art.pixilart.com/sr2cdfd4c74ed21.png')";
                break;
            case "snow":
                document.getElementById("meteoImage").src = "https://art.pixilart.com/sr291727443bfc4.png";
                document.body.style.backgroundImage = "url('https://art.pixilart.com/sr2a825e5d5fdb8.png')";
                break;
        }
        document.getElementById("tempText").textContent = t + "C";
        document.getElementById("humText").textContent = h + "%";
    </script>



</body>

</html>
)=====";
