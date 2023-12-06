module TwoBodyScattering

    export scattering, scatteringmovie, generate!, modulate, pitick, piticklabel, calc_avg

    #SIMULATION-RELATED FUNCTIONS

    #Function to evolve the two particles trajectories until their distance is lambda (interaction range)

    function scattering(phi10_::Float64, phi20_::Float64, lambda_::Float64, 
                        dtf_::Float64, intens_::Float64, v0_::Float64, diss_::Float64)
        
        phi1::Float64 = phi10_;
        phi2::Float64 = phi20_;
        phi1p::Float64 = phi1;
        phi2p::Float64 = phi2;
        alpha1::Float64 = phi10_;
        alpha2::Float64 = phi20_;
        r::Float64 = 1.0/lambda_-0.1;
        rp::Float64 = r;
        dt_::Float64 = dtf_;
        dtt_::Float64 = dt_;

        while r <= 1.0/lambda_

            rprime = diss_*exp(-lambda_*rp)/rp^4-v0_*(sin(phi1p)-sin(phi2p));
            phi1prime = -intens_*exp(-lambda_*rp)/rp^4*cos(phi1p)-v0_/rp*(cos(phi1p)-cos(phi2p));
            phi2prime = intens_*exp(-lambda_*rp)/rp^4*cos(phi2p)-v0_/rp*(cos(phi1p)-cos(phi2p));
            alpha1prime = -intens_*exp(-lambda_*rp)/rp^4*cos(phi1p);
            alpha2prime = intens_*exp(-lambda_*rp)/rp^4*cos(phi2p);

            r += dtt_*rprime;
            if phi1 < pi/2 && phi1 > -pi/2
                phi1 += dtt_*phi1prime;
                alpha1 += dtt_*alpha1prime;
            else
                phi1 += dtt_*phi1prime;
                alpha1 += dtt_*alpha1prime;
            end
            if phi2 < pi/2 && phi2 > -pi/2
                phi2 += dtt_*phi2prime;
                alpha2 += dtt_*alpha2prime;
            else
                phi2 += dtt_*phi2prime;
                alpha2 += dtt_*alpha2prime;
            end


            phi1 = modulate(phi1);
            phi2 = modulate(phi2);
            alpha1 = modulate(alpha1);
            alpha2 = modulate(alpha2);
            phi1p = phi1;
            phi2p = phi2;
            rp = r;
        end

        return (cos(phi10_)+cos(phi20_))*(cos(alpha1)+cos(alpha2)-cos(phi10_)-cos(phi20_))+
        (sin(phi10_)+sin(phi20_))*(sin(alpha1)+sin(alpha2)-sin(phi10_)-sin(phi20_))
    end

    # Generate positions and directons for two-particle scattering event

    function scatteringmovie(phi10_::Float64, phi20_::Float64, lambda_::Float64, 
        dtf_::Float64, intens_::Float64, v0_::Float64, diss_::Float64)
        phi1 = phi10_;
        phi2 = phi20_;
        phi1p = phi1;
        phi2p = phi2;
        alpha1 = phi10_;
        alpha2 = phi20_;
        r = 1.0/lambda_-1;
        rp = r;
        dt_ = dtf_;
        a1 = [];
        a2 = [];
        pos1x = 0.0;
        pos1y = r;
        pos2x = 0.0;
        pos2y = 0.0;
        modul::Float64 = deepcopy(r/10);

        push!(a1, [[pos1x,pos1y], [modul*cos(alpha1)+pos1x, -modul*sin(alpha1)+pos1y]])
        push!(a2, [[pos2x,pos2y],[modul*cos(alpha2)+pos2x, -modul*sin(alpha2)+pos2y]])

        while r <= 1.0/lambda_

        rprime = diss_*exp(-lambda_*rp)/rp^4-v0_*(sin(phi1p)-sin(phi2p));
        phi1prime = -intens_*exp(-lambda_*rp)/rp^4*cos(phi1p)-v0_/rp*(cos(phi1p)-cos(phi2p));
        phi2prime = intens_*exp(-lambda_*rp)/rp^4*cos(phi2p)-v0_/rp*(cos(phi1p)-cos(phi2p));
        alpha1prime = -intens_*exp(-lambda_*rp)/rp^4*cos(phi1p);
        alpha2prime = intens_*exp(-lambda_*rp)/rp^4*cos(phi2p);
        r += dt_*rprime;
        if phi1 < pi/2 && phi1 > -pi/2
        phi1 += dt_*phi1prime;#*intens_*exp(-lambda_*rp)/rp^4*cos(phi1p);
        alpha1 += dt_*alpha1prime;
        else
        phi1 += dt_*phi1prime;#*intens_*exp(-lambda_*rp)/rp^4*cos(phi1p);
        alpha1 += dt_*alpha1prime;
        end
        if phi2 < pi/2 && phi2 > -pi/2
        phi2 += dt_*phi2prime;#*intens_*exp(-lambda_*rp)/rp^4*cos(phi1p);
        alpha2 += dt_*alpha2prime;
        else
        phi2 += dt_*phi2prime;#*intens_*exp(-lambda_*rp)/rp^4*cos(phi1p);
        alpha2 += dt_*alpha2prime;
        end

        phi1 = modulate(phi1);
        phi2 = modulate(phi2);
        alpha1 = modulate(alpha1);
        alpha2 = modulate(alpha2);
        pos1x += dt_*v0_*cos(alpha1);
        pos1y += -dt_*v0_*sin(alpha1);
        pos2x += dt_*v0_*cos(alpha2);
        pos2y += -dt_*v0_*sin(alpha2);
        push!(a1, [[pos1x,pos1y], [modul*cos(alpha1)+pos1x, -modul*sin(alpha1)+pos1y]])
        push!(a2, [[pos2x,pos2y],[modul*cos(alpha2)+pos2x, -modul*sin(alpha2)+pos2y]])
        phi1p = phi1;
        phi2p = phi2;
        rp = r;
        end

        return (cos(phi10_)+cos(phi20_))*(cos(alpha1)+cos(alpha2)-cos(phi10_)-cos(phi20_))+
        (sin(phi10_)+sin(phi20_))*(sin(alpha1)+sin(alpha2)-sin(phi10_)-sin(phi20_)), phi1, phi2, a1, a2
    end

    #Simulate scattering events for different values of initial angles and save momentum changes

    function generate!(dinit_::Vector{Float64}, thetatot_::Vector{Float64}, deltaps_::Vector{Float64},
            dphi_::Float64,
            gamma_::Float64, v0_::Float64, int_range_::Float64, diss_::Float64,
            dt_::Float64)

        for p1 in -(pi-dphi_):dphi_:(pi-dphi_)
            for p2 in -(pi-dphi_):dphi_:(pi-dphi_)
                if p1 == pi/2 && p2 == pi/2 || p1 == -pi/2 && p2 == -pi/2 || p1 == pi/2 && p2 == -pi/2
                    continue
                end
                if (sin(p1)-sin(p2)) > 0 
                    push!(dinit_, p1-p2);
                    complex = exp(-p1*im)+exp(-p2*im);
                    push!(thetatot_, asin(imag(complex/abs(complex))));
                    push!(deltaps_, scattering(p1, p2, int_range_, dt_, gamma_, v0_, diss_));
                end
            end
        end

    end

    #Modulate angles with mod. 2pi in interval [-pi,pi]

    function modulate(phi_::Float64)
        if phi_ <= -pi
        phi_ = 2*pi+phi_;
        elseif phi_>= pi
        phi_ = -2*pi+phi_;
        end
        return phi_
    end 

    #PLOTTING-RELATED FUNCTIONS

    #Functions to make ticks in pi multiples for plots

    function pitick(start, stop, denom; mode=:text)
        a = Int(cld(start, π/denom))
        b = Int(fld(stop, π/denom))
        tick = range(a*π/denom, b*π/denom; step=π/denom)
        ticklabel = piticklabel.((a:b) .// denom, Val(mode))
        tick, ticklabel
    end
    
    function piticklabel(x::Rational, ::Val{:text})
        iszero(x) && return "0"
        S = x < 0 ? "-" : ""
        n, d = abs(numerator(x)), denominator(x)
        N = n == 1 ? "" : repr(n)
        d == 1 && return S * N * "π"
        S * N * "π/" * repr(d)
    end

    #Function to calculate average of pdp over different values of self-propulsion speed.

    function calc_avg(avgs_::Vector{Float64},
                        ranges_v_::Vector{Float64}, dphis_::Vector{Float64},
                        gamma_::Float64, diss_::Float64, lambda_::Float64,
                        dphi_::Float64)

        size::Int64 = length(dphis_);
        @simd for i in 1:length(ranges_v_)
            v = ranges_v_[i];
            avgpdp::Vector{Float64} = zeros(size);
            totn::Vector{Float64} = zeros(size);
            deltaps::Vector{Float64} = Float64[];
            thetatot::Vector{Float64} = Float64[]; 

            generate!(dinit, thetatot, deltaps,
                    dphi_,
                    gamma_, vv, lambda_, diss_,
                    dt);
            for x in 2:size
                for i in 1:length(dinit)
                    if modulate(dinit[i]) <= rangex[x] && modulate(dinit[i]) > rangex[x-1]
                        avgpdp[x]+=deltaps[i];
                        totn[x]+=1;
                    end
                end
            end
            avgpdp = avgpdp./totn;
            tot::Float64 = 0.0;
            for x in 1:size
                tot += dphi_*abs(sin(dphis_[x]/2))*avgdp[x]*v;
            end
            avgs_[i] = tot;
        end

    end
    
end