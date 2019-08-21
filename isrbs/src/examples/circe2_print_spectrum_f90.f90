
program circe2_print_spectrum_f90
  use kinds
  use circe2
  use tao_random_objects
  implicit none
  type(circe2_state) :: c2s
  type(rng_tao), save :: rng
  character(len=1024) :: filename, design, buffer
  integer :: status, nevents, seed
  real(kind=default) :: roots
  real(kind=default), dimension(2) :: x
  integer :: i, ierror
 
  design = "ILC"
  roots = 250.0D0
  filename = "250-TDR_ws-ee.circe2"
!  filename = "ilc250-ee.circe2"
!  filename = "ilc250.circe"
  nevents = 100


  call circe2_load (c2s, trim(filename), trim(design), roots, ierror)

  if (ierror /= 0) then
  print *, "circe2_generate: failed to load design ", trim(design), &
        " for ", real (roots, kind=single), &
        " GeV from ", trim(filename)
        stop
  end if

  call random2_seed (rng)

  print *,"circe2 file was loaded."
  do i = 1, nevents
      call circe2_generate (c2s, rng, x, [11, -11], [0, 0])
      write (*, '(F12.10,1X,F12.10)') x
  end do

  contains
     subroutine random2_seed (rng, seed)
       class(rng_tao), intent(inout) :: rng
       integer, intent(in), optional:: seed
       integer, dimension(8) :: date_time
       integer :: seed_value
       if (present (seed)) then
          seed_value = seed
       else
          call date_and_time (values = date_time)
          seed_value = product (date_time)
       endif
       call rng%init (seed_value)
     end subroutine random2_seed
end program circe2_print_spectrum_f90

