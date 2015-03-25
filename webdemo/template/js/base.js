$("#logout").click(function(event){
    event.preventDefault();
    del_cookie("admin_id");
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

$("form[data-type=formAction]").submit(function(event){
    event.preventDefault();
    var target = event.target;
    var action = $(target).attr("action");
    $.post(action, $(target).serialize(), function(ret){
        if(ret.Ret == "0") {
            alert(ret.Reason);
        } else {
            location.href = $(target).attr("form-rediret");
        }
    },"json")
})

$("form[data-type=getRegKeyAction]").submit(function(event){
		event.preventDefault();
		var target = event.target;
		var action = $(target).attr("action");
		$.post(action, $(target).serialize(), function(ret){
				if(ret.Ret == "0"){
						alert(ret.Reason);
				} else {
						$("input[name='reg_key']").val(ret.Reason);
						$("input[name='reg_mail2']").val($("input[name='reg_mail']").val());
				}
		}, "json")
})

$("form[data-type=regAccountAction]").submit(function(event){
			event.preventDefault();
			var target = event.target;
			var action = $(target).attr("action");
			var mail = $("input[name='reg_mail2']").val();
			var account = $("input[name='reg_account']").val();
			var password = $("input[name='reg_password']").val();
			var password2 = $("input[name='reg_password2']").val();

			if (password != password2){
				alert(" 两次输入密码不相符，请重新输入");
				return;
			}else{
				if (account.length < 5 ||
				account.length > 20) {
					alert("账户名在5-20个字符之间");
					return;
				}
			}
			
			if (!checkInputValid(account) ||
			!checkInputValid(password) ||
			!checkInputValid(password2)) {
				alert("输入必须为字母、数字及符号 (5-20)字符，请检查后重新输入");
				return
			}
			
			$.post(action, $(target).serialize(), function(ret){
				if(ret.Ret == "0"){
						alert(ret.Reason);
				} else {
						$("input[name='reg_key']").val(ret.Reason);
				}
			}, "json");
})