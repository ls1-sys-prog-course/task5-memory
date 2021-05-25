/// malloc 
#[no_mangle]
pub unsafe extern "C" fn malloc(size: libc::size_t) -> *mut libc::c_void {
    return std::ptr::null_mut();
}

/// free
#[no_mangle]
pub unsafe extern "C" fn free(p: *mut libc::c_void) {}

/// calloc
#[no_mangle]
pub unsafe extern "C" fn calloc(nobj: libc::size_t, size: libc::size_t) -> *mut libc::c_void {
    return std::ptr::null_mut();
}

/// realloc
#[no_mangle]
pub unsafe extern "C" fn realloc(p: *mut libc::c_void, size: libc::size_t) -> *mut libc::c_void {
    return std::ptr::null_mut();
}