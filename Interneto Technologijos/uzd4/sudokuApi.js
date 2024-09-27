export class SudokuApi {
    static async fetchBoardData() {
        return $.ajax({
            url: 'https://6550e0cc7d203ab6626e476a.mockapi.io/api/v1/SudokuBoard/1?fbclid=IwAR0Zs1QuyeuDFGTR5S-EaWCMw7mV3ExT6KTWMUvAF-tnt0xoIQqX6m3f9Ig',
            type: 'GET',
            dataType: 'json',
            error: () => { $.error('Unable to fetch board data.')}
        })
    }

    static async fetchBoardSolution() {
        return $.ajax({
            url: 'https://6550e0cc7d203ab6626e476a.mockapi.io/api/v1/SudokuSolutions/1?fbclid=IwAR1uZmkq26ItDU29_qR9VRA87BMH0vMyFLo5NdDOb-2EsGP8dH8aXC997Mw',
            type: 'GET',
            dataType: 'json',
            error: () => { $.error('Unable to fetch solution.')}
        }).then((res) => {
            return res.solution
        })
    }
}
