// set query type
let temperatureType = "C";

// initialize time variable
let time = 0;

// onclick handlers for celcius and fahrenheit buttons
$("#celcius-button").click(() => {
    temperatureType = "C"
    $("#temp-type").text("°C");
});
$("#fahrenheit-button").click(() => {
    temperatureType = "F"
    $("#temp-type").text("°F");
});

// Temperature function
function setTemperature(temperature) {

    // Celcius conditions
    if (temperatureType == "C") {
        if (temperature <= 10) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Cold!");
            $("i").removeClass().addClass("fa-solid fa-snowflake");
            $("#temp-icon").removeClass().addClass("text-cyan-400");
        } else if (temperature > 10 && temperature <= 15) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Cool");
            $("i").removeClass().addClass("fa-solid fa-wind");
            $("#temp-icon").removeClass().addClass("text-sky-400");
        } else if (temperature > 15 && temperature <= 25) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Perfect");
            $("i").removeClass().addClass("fa-solid fa-circle-dot");
            $("#temp-icon").removeClass().addClass("text-green-500");
        } else if (temperature > 25 && temperature <= 30) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Warm");
            $("i").removeClass().addClass("fa-solid fa-temperature-three-quarters");
            $("#temp-icon").removeClass().addClass("text-amber-500");
        } else if (temperature > 30 && temperature <= 35) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Hot");
            $("i").removeClass().addClass("fa-solid fa-temperature-full");
            $("#temp-icon").removeClass().addClass("text-orange-500");
        } else if (temperature > 35) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Fiery");
            $("i").removeClass().addClass("fa-solid fa-fire-flame-curved");
            $("#temp-icon").removeClass().addClass("text-red-600");
        }
    }
    
    // Fahrenheit conditions
    if (temperatureType == "F") {
        if (temperature <= 50) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Cold!");
            $("i").removeClass().addClass("fa-solid fa-snowflake");
            $("#temp-icon").removeClass().addClass("text-cyan-400");
        }
        else if (temperature > 50 && temperature <= 59) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Cool");
            $("i").removeClass().addClass("fa-solid fa-wind");
            $("#temp-icon").removeClass().addClass("text-sky-400");
        }
        else if (temperature > 59 && temperature <= 77) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Perfect");
            $("i").removeClass().addClass("fa-solid fa-circle-dot");
            $("#temp-icon").removeClass().addClass("text-green-500");
        }
        else if (temperature > 77 && temperature <= 86) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Warm");
            $("i").removeClass().addClass("fa-solid fa-temperature-three-quarters");
            $("#temp-icon").removeClass().addClass("text-amber-500");
        }
        else if (temperature > 86 && temperature <= 95) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Hot");
            $("i").removeClass().addClass("fa-solid fa-temperature-full");
            $("#temp-icon").removeClass().addClass("text-orange-500");
        }
        else if (temperature > 95) {
            $("#temp").text(`${temperature}`);
            $("#state").text("Fiery");
            $("i").removeClass().addClass("fa-solid fa-fire-flame-curved");
            $("#temp-icon").removeClass().addClass("text-red-600");
        }
    }
}

// interval for countdown bar that shows how many seconds left until new update. (to fix: not in sync)
const countdown = setInterval(function () {
    if (time == 5) {
        $("#timer").removeClass("bg-green-100 w-1/5").addClass("bg-green-100 w-full");
    }
    else if (time == 4) {
        $("#timer").removeClass("bg-green-100 w-full").addClass("bg-green-300 w-4/5");
    }
    else if (time == 3) {
        $("#timer").removeClass("bg-green-300 w-4/5").addClass("bg-green-500 w-3/5");
    }
    else if (time == 2) {
        $("#timer").removeClass("bg-green-500 w-3/5").addClass("bg-green-700 w-2/5");
    }
    else if (time == 1) {
        $("#timer").removeClass("bg-green-700 w-2/5").addClass("bg-green-900 w-1/5");
    } 
    else if (time == 0) {
        time = 5;
        countdown();
    }
    time -= 1;
}, 1000);


// Interval to update data temperature every 5 seconds
const interval = setInterval(function() {
    $.get(`/temperature?type=${temperatureType}`, function(data) {
        setTemperature(data.temperature);
        $("#sensorid").text(data.address);
    });

}, 5000);