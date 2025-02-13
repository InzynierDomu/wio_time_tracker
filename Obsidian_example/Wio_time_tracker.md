```dataviewjs
const pages = dv.pages('"Wio_time_tracker"')
const Names = pages.map(p => p.file.name).values
const work_Praca = pages.map(p => p.work_work_1).values
const meeting_Praca = pages.map(p => p.meeting_work).values

const chartData = {
    type: 'bar',
    data: {
        labels: Names, 
        datasets: [
            {
                label: 'work', 
                data: work_Praca, 
                backgroundColor: 'rgba(255, 99, 132, 0.2)', 
                borderColor: 'rgba(255, 99, 132, 1)',
                borderWidth: 1,
            },
            {
                label: 'meetings', 
                data: meeting_Praca, 
                backgroundColor: 'rgba(54, 162, 235, 0.2)', 
                borderColor: 'rgba(54, 162, 235, 1)', 
                borderWidth: 1,
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            legend: {
                position: 'top', 
            },
        },
        scales: {
            x: {
                stacked: true, 
            },
            y: {
                stacked: true, 
                beginAtZero: true, 
            }
        }
    }
}

window.renderChart(chartData, this.container)
```

```dataviewjs
const pages = dv.pages('"Wio_time_tracker"')

const Names = pages.map(p => p.file.name).values
const work_Project = pages.map(p => p.work_project).values

const chartData = {
    type: 'line', 
    data: {
        labels: Names, 
        datasets: [
            {
                label: 'Project time', 
                data: work_Project, 
                borderColor: 'rgba(75, 192, 192, 1)', 
                backgroundColor: 'rgba(75, 192, 192, 0.2)', 
                borderWidth: 2,
                fill: false, 
                tension: 0.3 
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            legend: {
                position: 'top',
            },
        },
        scales: {
            y: {
                beginAtZero: true, 
            }
        }
    }
}

window.renderChart(chartData, this.container)
```