<style>

p {
    color:white;
}

label {
    color:white;
}

ul {
    
  display: flex;
  flex-wrap: wrap;
  padding-left: 0;
  list-style: none;
}
li {
    
    margin: auto;
    padding: 10px;
    
}
li::before {
    
  display: inline-block;
  margin-right: 1ch;
  width: 1ch;
  text-align: center;
}

.img__wrap {
  position: relative;
}

.img__description {
  text-align: center;
  position: absolute;
  top: 0;
  bottom: 0;
  left: 0;
  right: 0;
  background: rgba(0, 0, 0, 0.72);
  color: #fff;
  visibility: hidden;
  opacity: 0;
  margin: 0;
  /* transition effect. not necessary */
  transition: opacity .2s, visibility .2s;
}

.img__wrap:hover .img__description {
  visibility: visible;
  opacity: 1;
  text-align: center;
}

.img__wrap:hover .delete_button {
  visibility: visible;
  opacity: 1;
  text-align: center;
  cursor:pointer;
}

#rec_list {
    display:inline-block;
}

.rec_status {
    position: absolute;
    top:0;
    left:0;
    bottom:0;
    right:0;
    margin:0;
}

.delete_button {
    position: absolute;
    top:0;
    right:0;
    margin:0;
    visibility: hidden;
    opacity: 0;
    z-index: 10;
    transition: opacity .2s, visibility .2s;
}

#stream_from {
    position: relative;
    text-align: center;
}

input {
    color-scheme: dark;
    appearance: none;
    background-clip: padding-box;
    line-height: 1.5;
    transition: border var(--timing-short) ease-in,background-color var(--timing-short) ease-in;
    border-style: solid;
    border-width: 2px;
    border-color: hsla(0,0%,100%,.16);
    background-color: hsla(0,0%,100%,.16);
}

button {
    color-scheme: dark;
    appearance: none;
    background-clip: padding-box;
    line-height: 1.5;
    transition: border var(--timing-short) ease-in,background-color var(--timing-short) ease-in;
    border-style: solid;
    border-width: 2px;
    border-color: hsla(0,0%,100%,.16);
    background-color: hsla(0,0%,100%,.16);
}

.session_buttons {
    border-radius: 10px;
}

button:hover {
    outline: none;
    border-color: hsla(0,0%,100%,.28);
}

button:focus {
    border-color: #9146ff;
    outline: none;
    background-color: black;
}

#search {
    font-size: 1.4rem;
    border-radius: 0.6rem 0px 0px 0.6rem;
}

#haur_input {
    border-radius: 0.6rem 0px 0px 0.6rem;
}

#second_input {
    border-radius: 0.0px 0.6rem 0.6rem 0px;
}

input:hover {
    outline: none;
    border-color: hsla(0,0%,100%,.28);
}

input:focus {
    border-color: #9146ff;
    outline: none;
    background-color: black;
}

#search_button {
    display: inline-flex;
  position: relative;
  -moz-box-align: center;
  align-items: center;
  -moz-box-pack: center;
  justify-content: center;
  vertical-align: middle;
  overflow: hidden;
  text-decoration: none;
  white-space: nowrap;
  user-select: none;
  border-top-right-radius: 0.6rem;
  border-bottom-right-radius: 0.6rem;
  background-color: #2e2e35;
  border: none;
  margin-left: 3px;
}

#search_button:hover {
    background-color: #575757;
}

progress {
    background-color: #575757;
    border-radius: 10rem;
}
progress::-moz-progress-bar {
    background: #9146ff; 
}

</style>

{#if is_auth }
    <button class="session_buttons" on:click={logout}>Выход</button>
{:else}
    <a href="/login">
        <button class="session_buttons">Вход</button>
    </a>
    <a href="/register">
        <button class="session_buttons">Регистрация</button>
    </a>
{/if}

<div id="stream_from">
    <form style="display:flexbox" on:submit|preventDefault={on_get_info}>
        <div style="display: inline-flex;">
        <input id="search" type="url" name="url" placeholder="Ссылка на трансляцию"><br>
        <button id="search_button" type="submit"><img src="icons8-search-50.png" alt="" height="auto" width="auto"></button>
        </div>
    </form>

    {#if stream_info}
    <form on:submit|preventDefault={onsubmit}>
        <br/>
        <div in:fade style="padding: 10px; display: inline-flex;">
            <img style="margin-right: 10px;" src="{stream_info.thumbnail}" height="200dp" width=auto alt="">
            <div>
                {#if stream_info.is_wait}
                    <p>До начала стрима: {stream_info.time_to_start}</p>
                {/if}
                <label for="format_select">качество:</label>
                <select name="format_id" on:change={(event)=>calc_size(event.target.selectedIndex)} id="format_select">
                    {#each stream_info.formats as {id,fps,tbr, resolution}}
                        <option value="{id}">{resolution}/({fps} fps)</option>
                    {/each}
                </select>
                <p>
                <lable>Время записи:</lable>
                <input id="haur_input" name="haur" size=2 type="number" value="{max_hour}" max="99">
                <input name="minute" size=2 type="number" value="{max_minutes}" max="59">
                <input id="second_input" type="number" name="second" size=2 value="{max_seconds}" max="59">
                </p>
                <p>Максимальное время записи: {max_hour}:{max_minutes}:{max_seconds}</p>
                <input value="начать запись" type="submit">
            </div>
        </div>
    </form>
    {/if}
    <p>{error}</p>
</div>

{#if user}
    <p>Доступное место: {humanFileSize((user.total_size / 8) * 1024)}/{humanFileSize(10737418240)}</p>
{/if}

{#if user}
    {#if records.length != 0}
    <p>Список записей:</p>
    {/if}
{/if}

<div id="rec_list">
    <ul> 
        {#if records}
            {#each records as {id, title, thumbnail, record_file,status}}
                <li>
                    <div class="img__wrap">
                        {#if status == "recording"}
                            <img class="rec_status" src="icons8-video-record-48.png" height="48dp;" width="auto" alt="">
                        {:else if status == "finished"}
                            <img class="rec_status" src="icons8-checkmark.svg" height="48dp;" width="auto" alt="">
                            <img on:click={delete_record(id)} class="delete_button" src="icons8-delete.svg" height="48dp;" width="auto" alt="">
                        {:else}
                            <img class="rec_status" src="icons8-clock-50.png" height="48dp;" width="auto" alt="">
                            {/if}
                        {#if record_file}
                            
                            <a href="/data/records/{record_file}">
                                <p class="img__description">{title}</p>
                            </a>
                        {:else}
                            <p class="img__description">{title}</p>
                        {/if}
                        {#if status != "wait"}
                            <img class="img__img" src="/data/thumbnails/{thumbnail}" alt="" width=auto height="200dp">
                        {:else}
                            <img class="img__img" src="{thumbnail}" alt="" width=auto height="200dp">
                        {/if}
                    </div>
                </li>
            {/each}
        {/if} 
    </ul>
</div>
<script>
// @ts-nocheck
    export const ssr = false;
    import {call} from "../rpc";
    import {get_userinfo, create_tmp_user} from "../user";
    import {fade} from 'svelte/transition';

    let records = [];
    let error = "";

	let user = null;

    let stream_info = null;

    let max_hour = 0;
    let max_minutes = 0;
    let max_seconds = 0;
    let current_url = null;
    let is_auth = false;

    if(typeof localStorage !== `undefined`){
        if(localStorage.getItem("token")){
            get_userinfo((data) => {
                user = data;
                is_auth = user != null && !user.is_tmp;
                console.log(is_auth);
            });

            call("/api/Record/list",{token: localStorage.getItem("token")},(data)=>{

                records = data["records"];
            },"GET");
        }
    }

    function calc_size(index){
        if(localStorage.getItem("token") == null || !is_auth){
            max_hour = 0;
            max_minutes = 10;
            max_seconds = 0;
            return;
        }

        const bitrate = stream_info.formats[index].bitrate;
        const seconds = 8388608 / bitrate;
        max_hour = Math.floor(seconds / 3600);
        max_minutes = Math.floor((seconds % 3600) / 60);
        max_seconds = Math.floor(((seconds % 3600) % 60));
    }

    function on_get_info(event){
        error = "";
		const formData = new FormData(event.target);
		const data = {};
		for (let field of formData) {
			const [key, value] = field;
			data[key] = value;
		}
        current_url = data["url"];
        call("/api/get_info",data,async (response)=>{
                event.target.reset();
                if("error" in response){
                    error = response["error"];
                    return;
                }
                stream_info = response;
                calc_size(0);
        });
    }

    function onsubmit(event){
			const formData = new FormData(event.target);
			const data = {};
			for (let field of formData) {
				const [key, value] = field;
				data[key] = value;
			}

      data["token"] = localStorage.getItem("token")
        event.target.reset();
     	if(localStorage.getItem("token") == null){
      	call("/api/User/create_tmp",null,(data)=>{
        	if("error" in data){
        		error = data["error"];
          	    return;
        	}
        	localStorage.setItem("token",data["token"]);
        	onsubmit(event);
      	});
        return;
        
      }

      const zeroPad = (num, places) => num.padStart(places, '0')
     	const time = `${zeroPad(data["haur"],2)}:${zeroPad(data["minute"],2)}:${zeroPad(data["second"],2)}`

      delete data["haur"];
      delete data["minute"];
      delete data["second"];
      data["time"] = time;
      data["is_wait"] = stream_info.is_wait;
      data["url"] = current_url;

      call("/api/Record/make",data,(response)=>{
        console.log(response);
        if("error" in response){
            error = response["error"];
            return;
        }
      	
        setTimeout(()=>{
        	get_userinfo((data) => user = data);
      	}, 5000);
      },"POST");
      stream_info = null;
    }

    function delete_record(id){
        call("/api/Record/remove",{token: localStorage.getItem("token"),id: id},()=>{
            call("/api/Record/list",{token: localStorage.getItem("token")},(data)=>{
                records = data["records"];
            },"GET");
        },"POST");
    }

    setInterval(()=>{
        if(localStorage.getItem("token") !== null){
            call("/api/Record/list",{token: localStorage.getItem("token")},(data)=>{
                records = data["records"];
            },"GET");
        }
    },5000);

    function logout(event){
        call("/api/User/logout",{token:localStorage.getItem("token")},(data)=>{
            if(data && data["error"]){
                alert(data["error"]);
            }else{
                user = null;
								is_auth = false;
                localStorage.removeItem("token");
                records = []
            }
        })
    }

    /**
     * Format bytes as human-readable text.
     * 
     * @param bytes Number of bytes.
     * @param si True to use metric (SI) units, aka powers of 1000. False to use 
     *           binary (IEC), aka powers of 1024.
     * @param dp Number of decimal places to display.
     * 
     * @return Formatted string.
     */
    function humanFileSize(bytes, si=false, dp=1) {
        const thresh = si ? 1000 : 1024;

        if (Math.abs(bytes) < thresh) {
            return bytes + ' B';
        }

        const units = ['КБ', 'МБ', 'ГБ', 'ТБ', 'ПБ', 'EB', 'ZB', 'YB'];
        let u = -1;
        const r = 10**dp;

        do {
            bytes /= thresh;
            ++u;
        } while (Math.round(Math.abs(bytes) * r) / r >= thresh && u < units.length - 1);


        return bytes.toFixed(dp) + ' ' + units[u];
    }

    // call("http://localhost:5000/get_info",{url:"https://www.youtube.com/watch?v=CJSfFzsU75g"},async (response)=>{
    //         stream_info = response;
    //         calc_size(0);
    //     });
</script>