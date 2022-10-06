use std::ptr;

use gl::types::{GLint, GLsizei};

pub struct Texture {
    width: u32,
    height: u32,
    f_handle: u32,
    handle: u32,
}

impl Texture {
    pub fn new(width: u32, height: u32) -> Self {
        let mut f_handle: u32 = 0;
        let mut handle: u32 = 0;
        let mut draw_buffers = [gl::COLOR_ATTACHMENT0];
        unsafe {
            gl::GenFramebuffers(1, &mut f_handle);
            gl::BindFramebuffer(gl::FRAMEBUFFER, f_handle);

            gl::GenTextures(1, &mut handle);
            gl::BindTexture(gl::TEXTURE_2D, handle);
            gl::TexImage2D(
                gl::TEXTURE_2D,
                0,
                gl::RGB as GLint,
                width as GLsizei,
                height as GLsizei,
                0,
                gl::RGB,
                gl::UNSIGNED_BYTE,
                ptr::null(),
            );

            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32);
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32);
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE as i32);
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE as i32);

            gl::FramebufferTexture(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, handle, 0);
            gl::DrawBuffers(1, draw_buffers.as_ptr());

            if gl::CheckFramebufferStatus(gl::FRAMEBUFFER) != gl::FRAMEBUFFER_COMPLETE {
                panic!("Could not create a framebuffer");
            }
        }

        Self {
            width,
            height,
            f_handle,
            handle,
        }
    }

    pub fn bind(&self) {
        unsafe {
            gl::BindFramebuffer(gl::FRAMEBUFFER, self.f_handle);
        }
    }
    pub fn unbind(&self) {
        unsafe {
            gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
        }
    }
}
