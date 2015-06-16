$("#logout").click(function(event){
    event.preventDefault();
    del_cookie("admin_name");
    window.location.href = "/login/index";
})

function del_cookie(name)
{
    document.cookie = name + '=; expires=Thu, 01 Jan 1970 00:00:01 GMT;path=/;';
}

function checkInputValid(inputContent) 
{
	if (!inputContent.match( /^[\u4E00-\u9FA5a-zA-Z0-9_]{5,20}$/)) {
		return false;
	} else {
 		return true;
	}
}

$("form[data-type=insertDonateAction]").submit(function(event){
			event.preventDefault();
			var target = event.target;
			var action = $(target).attr("action");
			var donatename = $("input[name='donate_name']").val();
			var donateorder = $("input[name='donate_order']").val();
			var donatemoney = $("input[name='donate_money']").val();
			
			if(donatename.length == 0 ||
			donateorder.length == 0 ||
			donatemoney.length == 0) {
				alert("输入不能为空!");
				return;
			}
			
			$.post(action, $(target).serialize(), function(ret){
				if(ret.Ret == "0"){
						$("#adddonate_result").text(ret.Reason)
				} else {
						$("#adddonate_result").text(ret.Reason)
				}
			}, "json");
})