(self.webpackChunk_N_E=self.webpackChunk_N_E||[]).push([[931],{9916:function(e,a,t){Promise.resolve().then(t.bind(t,5132)),Promise.resolve().then(t.bind(t,1355)),Promise.resolve().then(t.t.bind(t,2164,23)),Promise.resolve().then(t.t.bind(t,8518,23)),Promise.resolve().then(t.t.bind(t,231,23))},5132:function(e,a,t){"use strict";t.d(a,{default:function(){return p}});var l=t(7437),n=t(2265),i=t(3411),o=t.n(i),r=t(6648),s=e=>{let{isOpen:a,onClose:t,imageSrc:n}=e;return a?(0,l.jsx)("div",{className:o().modalOverlay,onClick:t,children:(0,l.jsx)("div",{className:o().modalContent,onClick:e=>e.stopPropagation(),children:(0,l.jsxs)("div",{className:o().imageWrapper,children:[" ",(0,l.jsx)(r.default,{src:n,alt:"Selected",className:o().modalImage})]})})}):null},d=t(182),c=t.n(d),m=t(6920),u=t(5079),p=e=>{let{images:a}=e,[t,i]=(0,n.useState)(a),[o,d]=(0,n.useState)(!1),[p,_]=(0,n.useState)(""),g=e=>{_(e),d(!0)},f=async e=>{if(!confirm("Are you sure you want to delete this image?"))return;let a=await fetch("/api/deleteImage",{method:"DELETE",headers:{"Content-Type":"application/json"},body:JSON.stringify({filename:e})});if(a.ok)alert("Image deleted successfully"),i(a=>a.filter(a=>a!==e));else{let e=await a.json();alert("Error deleting image: ".concat(e.message))}};return(0,l.jsxs)(l.Fragment,{children:[(0,l.jsx)("div",{className:c().imageList,children:t.map((e,a)=>(0,l.jsxs)("div",{className:c().imageWrapper,children:[(0,l.jsx)(r.default,{src:"/images/".concat(e),alt:"Uploaded image ".concat(e),width:300,height:200,className:c().image,onClick:()=>g("/images/".concat(e))}),(0,l.jsx)("button",{className:c().deleteButton,onClick:()=>f(e),children:(0,l.jsx)(m.G,{icon:u.I7k})})]},a))}),(0,l.jsx)(s,{isOpen:o,onClose:()=>{d(!1),_("")},imageSrc:p})," "]})}},1355:function(e,a,t){"use strict";var l=t(7437),n=t(2265),i=t(9259),o=t.n(i);a.default=()=>{let[e,a]=(0,n.useState)(null),[t,i]=(0,n.useState)(!1),r=async()=>{if(!e)return;i(!0);let t=new FormData;t.append("file",e);try{(await fetch("/api/upload",{method:"POST",body:t})).ok?(alert("Image uploaded successfully!"),a(null)):alert("Image upload failed.")}catch(e){console.error(e),alert("Image upload failed.")}finally{i(!1)}};return(0,l.jsxs)("div",{onDrop:e=>{e.preventDefault();let t=e.dataTransfer.files;t.length&&a(t[0])},onDragOver:e=>e.preventDefault(),className:o().dropZone,children:[e&&(0,l.jsx)("div",{className:o().fileInfo,children:(0,l.jsxs)("p",{children:["Selected file: ",e.name]})}),e?(0,l.jsxs)(l.Fragment,{children:[(0,l.jsx)("p",{children:"File ready for upload:"}),(0,l.jsx)("button",{type:"button",onClick:r,className:o().uploadButton,disabled:t,children:t?"Uploading...":"Upload File"})]}):(0,l.jsxs)(l.Fragment,{children:[(0,l.jsx)("p",{children:"Drag and drop an image here or click the button to select a file"}),(0,l.jsx)("input",{type:"file",accept:"image/*",onChange:e=>{a(e.target.files[0])},style:{display:"none"},id:"fileInput"}),(0,l.jsx)("label",{htmlFor:"fileInput",children:(0,l.jsx)("button",{type:"button",className:o().selectButton,onClick:()=>document.getElementById("fileInput").click(),children:"Select File"})})]})]})}},8518:function(e){e.exports={container:"page_container__jZF7q",imageGrid:"page_imageGrid___NnN_"}},182:function(e){e.exports={imageGrid:"Gallery_imageGrid__HKSzP",imageWrapper:"Gallery_imageWrapper__jOR1I",image:"Gallery_image__z292j",deleteButton:"Gallery_deleteButton__m4RaH"}},3411:function(e){e.exports={modalOverlay:"ImageModal_modalOverlay__miGic",modalContent:"ImageModal_modalContent__1_8tZ",imageWrapper:"ImageModal_imageWrapper__OFiHA",modalImage:"ImageModal_modalImage__O3ZZn"}},9259:function(e){e.exports={dropZone:"ImageUpload_dropZone__dz_nh",uploadButton:"ImageUpload_uploadButton__bDriJ"}},2164:function(e){e.exports={navbar:"Navbar_navbar__GrSXC",uploadButton:"Navbar_uploadButton__j3hhK",logo:"Navbar_logo__X4o9i",sticky:"Navbar_sticky__RqxnJ"}}},function(e){e.O(0,[957,676,839,971,23,744],function(){return e(e.s=9916)}),_N_E=e.O()}]);