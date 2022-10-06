use std::{ffi::CString, ptr};

pub struct Shader {
    pub handle: u32,
}

impl Shader {
    pub fn new() -> Self {
        let mut handle: u32 = 0;
        let mut vertex_handle: u32 = 0;
        let mut fragment_handle: u32 = 0;

        let vertex_shader = CString::new(
            r#"
            #version 330 core
            layout (location = 0) in vec3 aPos;

            void main() 
            {
                gl_Position = vec4(aPos, 1.0f);
            }
        "#
            .as_bytes(),
        )
        .unwrap();
        let fragment_shader = CString::new(
            r#"
            #version 330 core

            out vec4 color;

            void main() 
            {
                color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            }
        "#
            .as_bytes(),
        )
        .unwrap();

        unsafe {
            vertex_handle = gl::CreateShader(gl::VERTEX_SHADER);
            gl::ShaderSource(vertex_handle, 1, &vertex_shader.as_ptr(), ptr::null());
            gl::CompileShader(vertex_handle);

            let mut success = -1i32;
            let mut infoLog = [0i8; 128];
            gl::GetShaderiv(vertex_handle, gl::COMPILE_STATUS, &mut success);
            if success == 0 {
                gl::GetShaderInfoLog(vertex_handle, 128, ptr::null_mut(), infoLog.as_mut_ptr());
                let str = String::from_utf8(infoLog.iter().map(|&c| c as u8).collect()).unwrap();
                panic!("{}", str);
            }

            fragment_handle = gl::CreateShader(gl::FRAGMENT_SHADER);
            gl::ShaderSource(fragment_handle, 1, &fragment_shader.as_ptr(), ptr::null());
            gl::CompileShader(fragment_handle);

            gl::GetShaderiv(fragment_handle, gl::COMPILE_STATUS, &mut success);
            if success == 0 {
                gl::GetShaderInfoLog(fragment_handle, 128, ptr::null_mut(), infoLog.as_mut_ptr());
                let str = String::from_utf8(infoLog.iter().map(|&c| c as u8).collect()).unwrap();
                panic!("{}", str);
            }
            handle = gl::CreateProgram();
            gl::AttachShader(handle, vertex_handle);
            gl::AttachShader(handle, fragment_handle);
            gl::LinkProgram(handle);

            gl::GetProgramiv(handle, gl::LINK_STATUS, &mut success);
            if success == 0 {
                gl::GetProgramInfoLog(handle, 128, ptr::null_mut(), infoLog.as_mut_ptr());
                let str = String::from_utf8(infoLog.iter().map(|&c| c as u8).collect()).unwrap();
                panic!("{}", str);
            }
            gl::DeleteShader(vertex_handle);
            gl::DeleteShader(fragment_handle);
            gl::UseProgram(handle);
        }

        Self { handle }
    }

    pub fn use_program(&self) {
        unsafe {
            gl::UseProgram(self.handle);
        }
    }
}

impl Drop for Shader {
    fn drop(&mut self) {
        unsafe {
            gl::DeleteProgram(self.handle);
        }
    }
}
