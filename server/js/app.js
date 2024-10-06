function enableDeleteButton()
{
	document.getElementById("btnDlt").disabled = false;
}

// A point click event that uses the Renderer to draw a label next to the point
// On subsequent clicks, move the existing label instead of creating a new one.
Highcharts.addEvent(Highcharts.Point, 'click', function () {
    if (this.series.options.className.indexOf('popup-on-click') !== -1) {
        const chart = this.series.chart;
        const date = Highcharts.dateFormat('%A, %b %e, %Y', this.x);
        const text = `<b>${date}</b><br/>${this.series.name}: ${this.y}`;

        const anchorX = this.plotX + this.series.xAxis.pos;
        const anchorY = this.plotY + this.series.yAxis.pos;
        const align = anchorX < chart.chartWidth - 200 ? 'left' : 'right';
        const x = align === 'left' ? anchorX + 10 : anchorX - 10;
        const y = anchorY - 30;
        if (!chart.sticky) {
            chart.sticky = chart.renderer
                .label(text, x, y, 'callout',  anchorX, anchorY)
                .attr({
                    align,
                    fill: 'rgba(0, 0, 0, 0.75)',
                    padding: 10,
                    zIndex: 7 // Above series, below tooltip
                })
                .css({
                    color: 'white'
                })
                .on('click', function () {
                    chart.sticky = chart.sticky.destroy();
                })
                .add();
        } else {
            chart.sticky
                .attr({ align, text })
                .animate({ anchorX, anchorY, x, y }, { duration: 250 });
        }
    }
});


Highcharts.chart('container', {

    chart: {
        scrollablePlotArea: {
            minWidth: 700
        },
		zoomType:"xy"
    },

    data: {
        csvURL: './weather-report.csv',
        beforeParse: function (csv) {
            return csv.replace(/\n\n/g, '\n');
        },
		startRow: 1,
		firstRowAsNames: false
    },

    title: {
        text: 'Température et humidité au cours du temps',
        align: 'left'
    },

    xAxis: {
        // tickInterval: 7 * 24 * 3600 * 1000, // one week
        tickInterval: 3 * 60 * 60 * 1000, // three hours
        tickWidth: 1,
        gridLineWidth: 1,
        labels: {
            align: 'left',
            x: 3,
            y: 15
        }
    },

    yAxis: [{ // left y axis
        title: {
            text: "Temperature [°C]"
        },
        labels: {
            align: 'right',
            x: -3,
            y: 16,
            format: '{value} °C'
        },
        showFirstLabel: false
    }, { // right y axis
        gridLineWidth: 0,
        opposite: true,
        title: {
            text: "Humidité [%]"
        },
        labels: {
            align: 'left',
            x: 3,
            y: 16,
            format: '{value} %'
        },
        showFirstLabel: false
    }],

    legend: {
        align: 'right',
        verticalAlign: 'top',
        borderWidth: 0
    },

    tooltip: {
        shared: true,
        crosshairs: true
    },

    plotOptions: {
        series: {
            cursor: 'pointer',
            className: 'popup-on-click',
            marker: {
                lineWidth: 1
            }
        }
    },

    series: [{
        name: 'Température',
		yAxis: 0
    }, {
        name: 'Humidité',
		yAxis: 1
    }]
});
