const jquery = require("jquery")
$(document).ready(() => {
    $("#mybtn").click((err) => {
        err.preventDefault();
        $.post("/ChatRoomMessages", {id: "1234567890"}, (data, status) => {
            console.log(data);
            console.log(status);
        })
        // $.ajax({
        //     url: "/ChatRoom",
        //     type: "POST",
        //     dataType: "text",
        //     data: $("#mytxt").serialize(),
        //     success: (data) => {
        //         console.log(data);
        //     }
        // })
    })

})