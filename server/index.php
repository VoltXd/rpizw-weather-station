<!DOCTYPE html>

<html lang="fr">
    <head>
        <title>Météo France</title>
        <meta charset="utf-8"/>
        <link rel="stylesheet" href="css/style.css"/>
    </head>
    <body>
        <div id="wrapper">
            <div id="header">
                <h1>Météo France</h1>
				<a href="weather-report.csv">
                    <input type="button" name="buttonDownload" value="Télécharger" onclick="enableDeleteButton()"/>
				</a>
				<br/>
                
                <?php 
                if (isset($_POST['buttonDelete']))
                {
                    unlink("weather-report.csv");
                }
                ?>
				<form method="post">
					<input id="btnDlt" type="submit" name="buttonDelete" disabled value="Supprimer"/>
				</form>
            </div>
            <figure class="highcharts-figure">
                <div id="container"></div>
                <p class="highcharts-description">
                    Chart showing data loaded dynamically. The individual data points can
                    be clicked to display more information.
                </p>
            </figure>
        </div>


        <script src="https://code.highcharts.com/highcharts.js"></script>
        <script src="https://code.highcharts.com/modules/data.js"></script>
        <script src="https://code.highcharts.com/modules/series-label.js"></script>
        <script src="https://code.highcharts.com/modules/exporting.js"></script>
        <script src="https://code.highcharts.com/modules/export-data.js"></script>
        <script src="https://code.highcharts.com/modules/accessibility.js"></script>
        <script src="js/app.js" type="text/javascript"></script>
    </body>
</html>
